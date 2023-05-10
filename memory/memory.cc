#include "memory.h"

namespace Memory {

struct Header {
  std::byte* addr;
  std::size_t size;
  bool used;
  Header* next{nullptr};
  Header* prev{nullptr};
  std::type_index type{typeid(char)};

  Header(std::byte* addr, std::size_t size, bool used)
      : addr(addr), size(size), used(used) {}
};

constexpr const std::size_t HEADER_SIZE = sizeof(Header);

static std::byte* heap = nullptr;
static std::size_t heap_size = 0;
static Header* heap_head = nullptr;
static std::vector<Header*> vacant;

void cleanup() {
  if (heap) {
    std::free(heap);
  }
  heap = nullptr;
  heap_head = nullptr;
}

static void remove_free_block(const Header* block) noexcept {
  auto item = std::find(vacant.begin(), vacant.end(), block);
  if (item != vacant.end()) {
    vacant.erase(item);
  }
}

static void split_block(Header& block, std::size_t size) noexcept {
  auto* const pivot = block.addr + size;
  auto const dimension = block.size - size - HEADER_SIZE;

  auto* const header =
      new (pivot) Header(pivot + HEADER_SIZE, dimension, false);
  header->next = block.next;
  if (block.next) block.next->prev = header;
  block.next = header;
  header->prev = &block;

  block.size = size;

  vacant.push_back(header);
}

static void merge_block(Header& first, Header& second) noexcept {
  first.next = second.next;
  if (second.next) second.next->prev = &first;
  first.size += second.size + HEADER_SIZE;
  remove_free_block(&second);
}

void init(std::size_t size) {
  if (size < HEADER_SIZE) {
    std::cout << "You must specify the size of the allocated memory greater "
                 "than the size of the header equal to "
              << HEADER_SIZE << "\n";
    return;
  }
  if (heap) {
    std::free(heap);
  }
  heap = static_cast<std::byte*>(std::malloc(size));
  heap_size = size;

  heap_head =
      new (heap) Header(heap + HEADER_SIZE, heap_size - HEADER_SIZE, false);
  vacant.push_back(heap_head);
  std::atexit(cleanup);
}

void* malloc(std::size_t size) {
  for (auto* curr = heap_head; curr; curr = curr->next) {
    if (!curr->used && curr->size >= size) {
      if ((curr->size - size) >= HEADER_SIZE) {
        split_block(*curr, size);
      }
      remove_free_block(curr);
      curr->used = true;
      return static_cast<void*>(curr->addr);
    }
  }

  return nullptr;
}

void* calloc(std::size_t num, std::size_t size) {
  auto* const ptr = malloc(num * size);
  if (ptr) {
    std::memset(ptr, 0, num * size);
  }
  return ptr;
}

void* realloc(void* ptr, std::size_t size) {
  if (!ptr) return malloc(size);

  auto* const block = reinterpret_cast<Header*>(static_cast<std::byte*>(ptr) - HEADER_SIZE);
  if (block->size >= size) {
    if ((block->size - size) >= HEADER_SIZE) {
      split_block(*block, size);
    }
    return ptr;
  }

  if (block->next && !block->next->used && block->size + block->next->size + HEADER_SIZE >= size) {
    merge_block(*block, *block->next);
    if ((block->size - size) >= HEADER_SIZE) {
      split_block(*block, size);
    }
    return ptr;
  }

  auto* const modern = malloc(size);
  if (modern) {
    std::memcpy(modern, ptr, block->size);
    free(ptr);
    return modern;
  }

  return nullptr;
}

void free(void* ptr) noexcept {
  if (!ptr) return;

  auto* block = reinterpret_cast<Header*>(static_cast<std::byte*>(ptr) - HEADER_SIZE);
  block->used = false;

  if (block->prev && !block->prev->used) {
    merge_block(*block->prev, *block);
    block = block->prev;
  }

  if (block->next && !block->next->used) {
    merge_block(*block, *block->next);
  }

  vacant.push_back(block);
}

void* malloc_onlyfree(std::size_t size) {
  auto item = std::find_if(
      vacant.begin(), vacant.end(),
      [size](const Header* header) { return header->size >= size; });
  if (item != vacant.end()) {
    Header* const block = *item;
    vacant.erase(item);
    if ((block->size - size) >= HEADER_SIZE) {
      split_block(*block, size);
    }
    block->used = true;
    return static_cast<void*>(block->addr);
  }
  return nullptr;
}

void* calloc_onlyfree(std::size_t num, std::size_t size) {
  auto* const ptr = malloc_onlyfree(num * size);
  if (ptr) {
    std::memset(ptr, 0, num * size);
  }
  return ptr;
}

void* realloc_onlyfree(void* ptr, std::size_t size) {
  if (!ptr) return malloc_onlyfree(size);

  auto* const block = reinterpret_cast<Header*>(static_cast<std::byte*>(ptr) - HEADER_SIZE);
  if (block->size >= size) {
    if ((block->size - size) >= HEADER_SIZE) {
      split_block(*block, size);
    }
    return ptr;
  }

  if (block->next && !block->next->used && block->size + block->next->size + HEADER_SIZE >= size) {
    merge_block(*block, *block->next);
    if ((block->size - size) >= HEADER_SIZE) {
      split_block(*block, size);
    }
    return ptr;
  }

  auto* const modern = malloc_onlyfree(size);
  if (modern) {
    std::memcpy(modern, ptr, block->size);
    free(ptr);
    return modern;
  }

  return nullptr;
}

void free_onlyfree(void* ptr) { free(ptr); }

void defragmentation() {
  vacant.clear();
  Header* last = nullptr;
  for (Header* block = heap_head; block; block = block->next) {
    Header* potential = block->next;
    for (; potential && !potential->used; potential = potential->next)
      ;
    if (potential) {
      std::byte* insert = block->addr + block->size;
      if (!block->used) {
        insert = reinterpret_cast<std::byte*>(block);
        potential->prev = nullptr;
      }
      potential->addr = insert + HEADER_SIZE;
      block->next = reinterpret_cast<Header*>(insert);
      if (potential->prev) {
        potential->prev = block;
      }
      if (potential->next) {
        potential->next->prev = reinterpret_cast<Header*>(insert);
      }
      std::memmove(insert, reinterpret_cast<std::byte*>(potential),
                   potential->size + HEADER_SIZE);
      last = reinterpret_cast<Header*>(insert);
    }
  }
  if (last) {
    last->next = nullptr;
    std::size_t save_size = last->size;
    last->size =
        heap + heap_size - (reinterpret_cast<std::byte*>(last) + HEADER_SIZE);
    split_block(*last, save_size);
  }
}

template <typename T>
bool write(void* ptr, const std::vector<T>& src) {
  std::byte* start = reinterpret_cast<std::byte*>(ptr);
  auto size = sizeof(T) * src.size();
  for (Header* block = heap_head; block; block = block->next) {
    if ((block->addr <= start) && (start < (block->addr + block->size))) {
      if (block->used && block->size >= size) {
        block->type = std::type_index(typeid(T));
        if constexpr (std::is_trivially_copyable_v<T>) {
          std::memcpy(start, src.data(), size);
        } else {
          std::copy(src.begin(), src.end(), reinterpret_cast<T*>(start));
        }
        return true;
      }
      return false;
    }
  }
  return false;
}

template bool write<char>(void*, const std::vector<char>&);
template bool write<int>(void*, const std::vector<int>&);
template bool write<double>(void*, const std::vector<double>&);

void dump() {
  for (auto* block = heap_head; block; block = block->next) {
    std::cout << block->addr << '\n';

    std::cout << "\tContent: [";
    if (block->type == std::type_index(typeid(int))) {
      auto* data = reinterpret_cast<int*>(block->addr);
      std::size_t size = block->size / sizeof(int);
      std::copy_n(data, size, std::ostream_iterator<int>(std::cout, ", "));
    } else if (block->type == std::type_index(typeid(double))) {
      auto* data = reinterpret_cast<double*>(block->addr);
      std::size_t size = block->size / sizeof(double);
      std::copy_n(data, size, std::ostream_iterator<double>(std::cout, ", "));
    } else {
      auto* data = reinterpret_cast<char*>(block->addr);
      std::size_t size = block->size / sizeof(char);
      std::copy_n(data, size, std::ostream_iterator<char>(std::cout, ", "));
    }
    std::cout << "]\n";

    std::cout << "\tSize: " << block->size << '\n';
    std::cout << "\tState: " << block->used << '\n';
  }
}

}  // namespace Memory