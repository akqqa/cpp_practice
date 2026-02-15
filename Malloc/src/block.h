#ifndef BLOCK_H
#define BLOCK_H

#include <cstddef>

struct alignas(std::max_align_t) Block { // always align the block size
    std::size_t size;   // Size of usable memory
    bool free;          // Is this block free?
    Block* next;        // Next block in linked list
};

#endif