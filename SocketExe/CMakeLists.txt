cmake_minimum_required(VERSION 3.29)
project(SocketExe)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(externals)
add_subdirectory(Socket)
add_subdirectory(Client)
add_subdirectory(Server)

if(WIN32)
    target_link_libraries(Socket PRIVATE ws2_32)
endif()

target_link_libraries(Socket PUBLIC fmt::fmt-header-only)

enable_testing()
add_test(NAME SocketTest COMMAND bash ${CMAKE_SOURCE_DIR}/SocketExe/run_test.sh)
set_tests_properties(SocketTest PROPERTIES ENVIRONMENT
        "SERVER_EXECUTABLE=$<TARGET_FILE:Server>;CLIENT_EXECUTABLE=$<TARGET_FILE:Client>"
)