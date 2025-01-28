cmake_minimum_required(VERSION 3.29)
project(SocketExe)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Socket)
add_subdirectory(Client)
add_subdirectory(Server)

if(WIN32)
    target_link_libraries(Socket PRIVATE ws2_32)
endif()

enable_testing()
add_test(NAME SocketTest COMMAND ${CMAKE_COMMAND} -E env "SERVER_EXECUTABLE=$<TARGET_FILE:Server>" "CLIENT_EXECUTABLE=$<TARGET_FILE:Client>" ${CMAKE_SOURCE_DIR}/run_test.sh)
