find_package(Catch2 3 REQUIRED)
add_executable(filesystem_tests filesystem_tests.cpp)
target_link_libraries(filesystem_tests PRIVATE Catch2::Catch2WithMain filesystem)

include(CTest)
include(Catch)
catch_discover_tests(filesystem_tests)