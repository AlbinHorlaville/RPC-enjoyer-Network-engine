cmake_minimum_required(VERSION 3.29)
project(Stream)

set(CMAKE_CXX_STANDARD 20)

add_library(Stream Stream.cpp)

target_include_directories(Stream PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Stream Socket)
target_include_directories(Stream PRIVATE ${CMAKE_SOURCE_DIR}/Socket)

target_link_libraries(Stream tools)
target_include_directories(Stream PRIVATE ${CMAKE_SOURCE_DIR}/tools)