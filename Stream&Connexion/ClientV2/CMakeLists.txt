cmake_minimum_required(VERSION 3.29)
project(ClientV2)

set(CMAKE_CXX_STANDARD 20)

add_executable(ClientV2 Main.cpp Client.cpp)

target_link_libraries(ClientV2 Stream)
target_include_directories(ClientV2 PRIVATE ${CMAKE_SOURCE_DIR}/Stream)

target_link_libraries(ClientV2 Socket)
target_include_directories(ClientV2 PRIVATE ${CMAKE_SOURCE_DIR}/Socket)

target_link_libraries(ClientV2 tools)
target_include_directories(ClientV2 PRIVATE ${CMAKE_SOURCE_DIR}/tools)