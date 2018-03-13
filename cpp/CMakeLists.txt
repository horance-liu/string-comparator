project(string-comparator)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE all_files
src/*.cpp
src/*.cc
src/*.c
test/*.cpp
test/*.cc
test/*.c)

add_executable(string-comparator-test ${all_files})

target_link_libraries(string-comparator-test cut)