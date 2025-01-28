cmake_minimum_required(VERSION 3.29)
project(Client)

set(CMAKE_CXX_STANDARD 20)

#add_subdirectory(../Socket ${CMAKE_BINARY_DIR}/Socket_Client)

add_executable(Client Client.cpp)

target_link_libraries(Client Socket)
target_include_directories(Client PRIVATE ${CMAKE_SOURCE_DIR}/Socket)

