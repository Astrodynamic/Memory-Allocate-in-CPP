#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <typeindex>
#include <vector>

namespace Memory {

void init(std::size_t size);

void* malloc(std::size_t size);
void* calloc(std::size_t num, std::size_t size);
void* realloc(void* ptr, std::size_t size);
void free(void* ptr) noexcept;

void* malloc_onlyfree(std::size_t size);
void* calloc_onlyfree(std::size_t num, std::size_t size);
void* realloc_onlyfree(void* ptr, std::size_t size);
void free_onlyfree(void* ptr);

void defragmentation();

template <typename T>
bool write(void* ptr, const std::vector<T>& src);

void dump();

}  // namespace Memory