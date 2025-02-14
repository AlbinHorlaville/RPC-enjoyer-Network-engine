cmake_minimum_required(VERSION 3.29)
project(ClientV2)

set(CMAKE_CXX_STANDARD 20)

add_executable(ClientV2 Main.cpp)

target_link_libraries(ClientV2 Stream)
target_include_directories(ClientV2 PRIVATE ${CMAKE_SOURCE_DIR}/Stream)

target_link_libraries(ClientV2 Format)
target_include_directories(ClientV2 PRIVATE ${CMAKE_SOURCE_DIR}/Format)