#pragma once

#include <cstddef>
#include "block.h"

void init_allocator();
void* my_malloc(std::size_t size);
void my_free(void* ptr);
void dump_heap();
int free_list_size();
