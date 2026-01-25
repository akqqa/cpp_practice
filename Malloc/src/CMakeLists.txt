add_library(my_malloc my_malloc.cpp)
target_include_directories(my_malloc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})