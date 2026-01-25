#include <cstddef>
#include <cstdint>
#include <cstdio>
#include "block.h"
#include "block_sort.h"

static constexpr std::size_t HEAP_SIZE = 1024 * 1024; // 1 MB

static std::uint8_t heap[HEAP_SIZE];

// Head of the block list
static Block* free_list = nullptr; // This is a pointer to the first block in the heap

//todo - add sorting for largest first, and other allocation schemes
// wait i just need a linear scan not merge sort oops

void init_allocator() {
    // Initialize a single large free block covering the whole heap
    free_list = reinterpret_cast<Block*>(heap); // SETS THE FIRST X BYTES OF THE HEAP TO THE FIRST BLOCK
    free_list->size = HEAP_SIZE - sizeof(Block); // THE SIZE OF THIS BLOCK (NOT HEADER) IS THE HEAP MINUS THE HEADER
    free_list->free = true;
    free_list->next = nullptr;
}

void* my_malloc(std::size_t size) { // Size is size in bytes
    if (size == 0) {
        return nullptr;
    }

    // Get the current block pointer
    Block* current = free_list;

    // Will loop through all blocks until one free and right size is found
    while (current) {
        if (current->free && current->size >= size) { // If current block is big enough and free, allocate block
            // TODO:
            // User alignment

            // If enough space, split the block
            if (current->size > size + sizeof(Block) + 1) { 
                Block* next = reinterpret_cast<Block*>(
                    reinterpret_cast<std::uint8_t*>(current) + sizeof(Block) + size
                );
                next->size = current->size - size - sizeof(Block);
                next->free = true;
                next->next = current->next;
                current->next = next;
                current->size = size;
            }

            current->free = false;
            return reinterpret_cast<void*>(
                reinterpret_cast<std::uint8_t*>(current) + sizeof(Block) // Return pointer to start of free memory
            );
        }
        current = current->next; // Move to next block
    }

    // No suitable block found
    return nullptr;
}

void my_free(void* ptr) {
    if (!ptr) {
        return;
    }

    Block* block = reinterpret_cast<Block*>(
        reinterpret_cast<std::uint8_t*>(ptr) - sizeof(Block) // Turn the pointer to the start of the memory space into a uint8_t, and go back sizeof(Block) bytes to get the address of the header
    );

    block->free = true;
    
    // Check if next block is free and if so, coalesce
    if (block->next && block->next->free) {
        block->size += block->next->size + sizeof(Block);
        block->next = block->next->next;
    }
    // Check if previous block is free and if so, coalesce
    Block* current = free_list;
    while (current and current->next) {
        if (current->next == block) {
            if (current->free) {
                current->next = block->next;
                current->size += block->size + sizeof(Block);
                // Change block to current in case used later on.
                block = current;
            } else {
                break;
            }
        }
        current = current->next; // Move to next block
    }
}

void dump_heap() {
    Block* current = free_list;
    std::size_t index = 0;

    while (current) {
        std::printf(
            "Block %zu | addr=%p | size=%zu | free=%d\n",
            index++, (void*)current, current->size, current->free
        );
        current = current->next;
    }
    printf("\n");
}

int main() {
    init_allocator();

    dump_heap();

    void* a = my_malloc(64);
    void* b = my_malloc(128);

    if(a == nullptr) {
        printf("a is null\n");
    }
    if(b == nullptr) {
        printf("b is null\n");
    }

    dump_heap();

    my_free(a);

    void* c = my_malloc(999999);

    dump_heap();

    my_free(b);

    dump_heap();

    my_free(c);

    dump_heap();

    return 0;
}
