cmake_minimum_required(VERSION 3.29)
project(ServerV2)

set(CMAKE_CXX_STANDARD 20)

add_executable(ServerV2 Main.cpp)

target_link_libraries(ServerV2 Stream)
target_include_directories(ServerV2 PRIVATE ${CMAKE_SOURCE_DIR}/Stream)

target_link_libraries(ServerV2 Format)
target_include_directories(ServerV2 PRIVATE ${CMAKE_SOURCE_DIR}/Format)

