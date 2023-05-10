# MemoryLogic in CPP

This project is an implementation of a memory management library written in C++. The library provides functionality for allocating and freeing memory, and it includes implementations of the `malloc`, `calloc`, `realloc`, and `free` functions.

## Table of Contents

- [MemoryLogic in CPP](#memorylogic-in-cpp)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Building the Project](#building-the-project)
  - [Dependencies](#dependencies)
  - [Usage](#usage)
    - [Function description](#function-description)
  - [License](#license)

## Introduction

This project is an implementation of a memory management library. The goal of the project is to provide a library that can allocate and free memory for other programs. The library includes implementations of the `malloc`, `calloc`, `realloc`, and `free` functions.

## Building the Project

To build the project, simply run the following commands:

```shell
cmake -S . -B ./build
cmake --build ./build
```

This will create a static library `memory.a` in the `lib` directory.

To rebuild the project, run:

```shell
make rebuild
```

This will clean the previous build and install the new version of the library.

## Dependencies

The project requires the following dependencies:

- CMake 3.15 or later
- C++17

## Usage

To use the library in your own project, include the header file `memory.h` and link with the static library `memory.a`.

Here is an example of how to use the `malloc` and `free` functions provided by the library:

```cpp
#include <iostream>
#include "memory.hpp"

int main() {
    // Allocate memory for an array of 10 integers
    int* arr = static_cast<int*>(Memory::malloc(10 * sizeof(int)));

    // Set the values of the array
    for (int i = 0; i < 10; ++i) {
        arr[i] = i;
    }

    // Print the values of the array
    for (int i = 0; i < 10; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    // Free the memory allocated for the array
    Memory::free(arr);

    return 0;
}
```

For more examples of how to use the library, see the `examples` directory.

### Function description

| № | Function | Description |
| ------ | ----------------------------------------------------- | ------ |
| 1 | `void *malloc (size_t size)` | The `malloc` function allocates a `size` byte memory block and returns a pointer to the beginning of the block. The contents of the allocated memory block are not initialized; they remain with undefined values. If it fails, it returns a null pointer. | 
| 2 | `void *calloc (size_t num, size_t size)` | The `calloc` function allocates a block of memory to an array of `num` elements, where each element is of `size` bytes, and initializes all its bits with zeros. As a result, a block of `num * size` bytes is allocated, and the whole block is filled with zeros. It returns a pointer to the beginning of the block; if it fails, it returns a null pointer. |
| 3 | `void *realloc(void *ptr, size_t size)` | The `realloc` function reallocates memory blocks. The size of the memory block pointed to by the `ptr` is changed to `size` bytes. A memory block can decrease or increase in size. This function can move the memory block to a new location, in which case the function returns a pointer to the new memory location. The contents of the memory block are maintained even if the new block is smaller than the old one. Only the data that does not fit into the new block is discarded. If the new `size` value is larger than the old one, the contents of the newly allocated memory will be undefined. Returns a pointer to the beginning of the block, with the original `ptr` pointer becoming invalid and any access to it being undefined behavior. In case of an error it returns a null pointer and the original `ptr` pointer remains valid. |
| 4 | `void free (void* ptr)` | The `free` function frees the memory space. A block of memory previously allocated by calling `malloc`, `calloc` or `realloc` is released. That means that the freed memory can be further used by programs or the OS. Note that this function leaves the value of `ptr` unchanged, so it still points to the same memory block and not to a null pointer. |
| 5 | `void *s21_malloc_onlyfree (size_t size)` | The `s21_malloc_onlyfree` function searches for a free memory block of `size` bytes, only considering free blocks. If a suitable block is found, it returns a pointer to the beginning of the block. If it fails, it returns a null pointer. |
| 6 | `void *s21_calloc_onlyfree (size_t num, size_t size)` | The `s21_calloc_onlyfree` function searches for a free memory block to an array of `num` elements, where each element is of `size` bytes, only considering free blocks. It initializes all its bits with zeros. As a result, a block of `num * size` bytes is allocated, and the whole block is filled with zeros. It returns a pointer to the beginning of the block; if it fails, it returns a null pointer. |
| 7 | `void *s21_realloc_onlyfree(void *ptr, size_t size)` | The `s21_realloc_onlyfree` function reallocates memory blocks, only considering free blocks. The size of the memory block pointed to by the `ptr` is changed to `size` bytes. A memory block can decrease or increase in size. This function can move the memory block to a new location, in which case the function returns a pointer to the new memory location. The contents of the memory block are maintained even if the new block is smaller than the old one. Only the data that does not fit into the new block is discarded. If the new `size` value is larger than the old one, the contents of the newly allocated memory will be undefined. Returns a pointer to the beginning of the block, with the original `ptr` pointer becoming invalid and any access to it being undefined behavior. In case of an error it returns a null pointer and the original `ptr` pointer remains valid. |
| 8 | `void s21_free_onlyfree (void* ptr)` | The `s21_free_onlyfree` function frees the memory space. A block of memory previously allocated by calling `s21_malloc`, `s21_calloc` or `s21_realloc` is released, only considering free blocks. That means that the freed memory can be further used by programs or the OS. Note that this function leaves the value of `ptr` unchanged, so it still points to the same memory block and not to a null pointer. |
| 9 | `void s21_defragmentation()` | The `s21_defragmentation` function defragments the current heap. It combines all free blocks into one larger block of memory, reducing external fragmentation. The function also updates the metadata of the blocks, including the `content_type` field if necessary. |
| 10 | `template <typename T> bool write(void* ptr, const std::vector<T>& src)` | This function writes a `std::vector` of type `T` to a block of memory starting at `ptr`. The memory block must have enough space to fit the entire vector. If the write is successful, the function returns `true`; otherwise, it returns `false`. The `T` type must be trivially copyable or have a `std::copy`-compatible iterator. |
| 11 | `void dump()` | This function dumps information about all allocated memory blocks to the standard output. For each block, it prints its starting address, content (as a comma-separated list), size, and state (used or not used). It also prints the type of the block, which is set by the `write` function. If the block contains non-trivially copyable types, the content is printed as a list of characters. |

Note that these two functions are not standard library functions, but rather appear to be part of a custom memory management system implemented by the user.


## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.