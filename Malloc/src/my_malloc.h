#pragma once

#include <cstddef>

void init_allocator();
void* my_malloc(std::size_t size);
void my_free(void* ptr);
void dump_heap();
