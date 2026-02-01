#pragma once

#include <cstddef>
#include "block.h"

void init_allocator();
void* my_malloc(std::size_t size);
void my_free(void* ptr);
void dump_heap();
std::string dump_heap_string();
int free_list_size();
Block* getBlock(int index);
