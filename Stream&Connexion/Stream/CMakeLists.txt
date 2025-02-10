cmake_minimum_required(VERSION 3.29)
project(Stream)

set(CMAKE_CXX_STANDARD 20)

target_link_libraries(Stream Socket)
target_include_directories(Stream PRIVATE ${CMAKE_SOURCE_DIR}/Socket)

add_library(Stream Stream.cpp)
