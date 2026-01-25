find_package(Catch2 3 REQUIRED)
add_executable(my_malloc_tests my_malloc_tests.cpp)
target_link_libraries(my_malloc_tests PRIVATE Catch2::Catch2WithMain my_malloc)

include(CTest)
include(Catch)
catch_discover_tests(my_malloc_tests)