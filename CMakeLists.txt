cmake_minimum_required(VERSION 3.4)
project(cs564_project_3)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_library(
        join
        src/join.cpp
        src/join.hpp
        src/file.cpp
        src/file.hpp
)

add_executable(test_file src/test/test_file.cpp)
target_include_directories(test_file PRIVATE src)
target_link_libraries(test_file join)
add_test(test_file test_file)

add_executable(test_join src/test/test_join.cpp)
target_include_directories(test_join PRIVATE src)
target_link_libraries(test_join join)
add_test(test_join test_join)
