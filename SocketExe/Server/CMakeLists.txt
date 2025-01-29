cmake_minimum_required(VERSION 3.29)
project(Server)

set(CMAKE_CXX_STANDARD 20)

add_executable(Server Server.cpp)

target_link_libraries(Server Socket)
target_include_directories(Server PRIVATE ${CMAKE_SOURCE_DIR}/Socket)

