cmake_minimum_required(VERSION 3.29)
project(Format)

set(CMAKE_CXX_STANDARD 20)

add_library(Format INTERFACE)
target_include_directories(Format INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})