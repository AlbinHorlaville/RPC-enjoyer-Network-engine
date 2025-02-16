cmake_minimum_required(VERSION 3.29)
project(tools)

set(CMAKE_CXX_STANDARD 20)

add_library(tools INTERFACE FormatPackage.h Timer.h)
target_include_directories(tools INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})