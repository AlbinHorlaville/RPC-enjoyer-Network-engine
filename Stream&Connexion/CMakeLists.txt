cmake_minimum_required(VERSION 3.29)
project(Stream&Connexion)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(ClientV2)
add_subdirectory(tools)
add_subdirectory(ServerV2)
add_subdirectory(Stream)

enable_testing()