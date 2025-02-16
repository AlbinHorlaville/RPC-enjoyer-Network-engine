cmake_minimum_required(VERSION 3.29)
project(ServerV2)

set(CMAKE_CXX_STANDARD 20)

add_executable(ServerV2 Main.cpp Server.cpp)

target_link_libraries(ServerV2 PRIVATE Stream tools Socket)  # Ensure Socket is linked last
target_include_directories(ServerV2 PRIVATE ${CMAKE_SOURCE_DIR}/Stream)
target_include_directories(ServerV2 PRIVATE ${CMAKE_SOURCE_DIR}/tools)
target_include_directories(ServerV2 PRIVATE ${CMAKE_SOURCE_DIR}/Socket)

