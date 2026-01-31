#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "my_malloc.h"
#include "allocator_config.h"

// Basic malloc tests:
// Allocation returns a pointer
// When allocated, there is one blcok in the free list that is allocated, with another empty block. size of the blocks is expected
// splitting and coalescing

TEST_CASE("allocation returns usable pointer") {
    init_allocator();
    void* a = my_malloc(64);
    
    REQUIRE(a != nullptr);

    auto* bytes = static_cast<std::uint8_t*>(a);
    for (std::size_t i = 0; i < 64; ++i) {
        bytes[i] = 0xAB;
    }

    my_free(a);
}

TEST_CASE("allocator splits blocks") {
    init_allocator();

    void* a = my_malloc(64);
    void* b = my_malloc(256);

    REQUIRE(free_list_size() == 3);
}

TEST_CASE("allocator coalesces") {
    init_allocator();

    void* a = my_malloc(64);
    void* b = my_malloc(256);

    my_free(a);

    // a is free, b taken, rest is free, so hasnt coalesced yet
    REQUIRE(free_list_size() == 3);

    my_free(b);

    // now all 3 blocks will coalesce into 1
    REQUIRE(free_list_size() == 1);
    
}

TEST_CASE("allocation marks block correctly") {
    init_allocator();

    void* a = my_malloc(64);

    auto* block = reinterpret_cast<Block*>(
        static_cast<std::uint8_t*>(a) - sizeof(Block)
    );

    REQUIRE(block->free == false);
    REQUIRE(block->size == 64);

    my_free(a);
}

TEST_CASE("allocation leaves empty block correct") {
    init_allocator();

    void* a = my_malloc(64);

    auto* block = reinterpret_cast<Block*>(
        static_cast<std::uint8_t*>(a) - sizeof(Block)
    );
    block = block->next;

    REQUIRE(block->free == true);
    CAPTURE(block->size);
    REQUIRE(block->size == HEAP_SIZE - 64 - sizeof(Block) - sizeof(Block));

    my_free(a);
}

