cmake_minimum_required(VERSION 3.29)
project(SocketExe)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Socket)
add_subdirectory(Client)
add_subdirectory(Server)

if(WIN32)
    target_link_libraries(Socket PRIVATE ws2_32)
endif()

add_test(NAME TestServerClient COMMAND Server && Client "127.0.0.1" "Albin")

set (passRegex 1)
set (failRegex 0)

set_property (TEST TestServerClient
        PROPERTY PASS_REGULAR_EXPRESSION "${passRegex}")
set_property (TEST TestServerClient
        PROPERTY FAIL_REGULAR_EXPRESSION "${failRegex}")