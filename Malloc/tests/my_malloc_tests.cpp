#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <thread>
#include "my_malloc.h"
#include "allocator_config.h"

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

TEST_CASE("allocator returns null when no space left in heap") {
    init_allocator();
    void* a = my_malloc(HEAP_SIZE - sizeof(Block));
    
    REQUIRE(a != nullptr);
    REQUIRE(free_list_size() == 1);

    void* b = my_malloc(4);
    REQUIRE(b == nullptr);
    REQUIRE(free_list_size() == 1);

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

    CAPTURE(dump_heap_string());

    Block* block = getBlock(0);

    REQUIRE(block->free == false);
    REQUIRE(block->size == 64);

    my_free(a);
}

TEST_CASE("allocation leaves empty block correct") {
    init_allocator();

    void* a = my_malloc(64);
    
    CAPTURE(dump_heap_string());

    Block* block = getBlock(1);

    REQUIRE(block->free == true);
    CAPTURE(block->size);
    REQUIRE(block->size == HEAP_SIZE - 64 - sizeof(Block) - sizeof(Block));

    my_free(a);
}

void threadFunc() {
        for (int i = 0; i < 50; ++i) {
            void* p = my_malloc(50);
            std::this_thread::yield();
        }
}

// Run multiple times to check for race conditions - not perfect but better than nothing
TEST_CASE("allocation is thread safe") {
    init_allocator();

    std::thread t1(threadFunc);
    std::thread t2(threadFunc);
    std::thread t3(threadFunc);

    t1.join();
    t2.join();
    t3.join();

    CAPTURE(dump_heap_string());

    REQUIRE(free_list_size() == 151);
}

TEST_CASE("Returned pointers are correctly aligned") {
    init_allocator();

    void* a = my_malloc(2343);
    void* b = my_malloc(123);
    void* c = my_malloc(6767);
    void* d = my_malloc(3);

    REQUIRE(free_list_size() == 5);

    // Check each one is aligned
    REQUIRE(reinterpret_cast<std::uintptr_t>(a) % ALIGNMENT_SIZE == 0);
    REQUIRE(reinterpret_cast<std::uintptr_t>(b) % ALIGNMENT_SIZE == 0);
    REQUIRE(reinterpret_cast<std::uintptr_t>(c) % ALIGNMENT_SIZE == 0);
    REQUIRE(reinterpret_cast<std::uintptr_t>(d) % ALIGNMENT_SIZE == 0);



}