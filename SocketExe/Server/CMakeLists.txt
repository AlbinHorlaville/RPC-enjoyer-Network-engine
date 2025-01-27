cmake_minimum_required(VERSION 3.29)
project(SocketExe)

set(CMAKE_CXX_STANDARD 20)

add_executable(SocketExe Server.cpp)

target_link_libraries(SocketExe PUBLIC Socket)
target_include_directories(SocketExe PUBLIC
        "${PROJECT_BINARY_DIR}"
        "../Socket"
)

