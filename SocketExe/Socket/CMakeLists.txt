cmake_minimum_required(VERSION 3.29)
project(Socket)

set(CMAKE_CXX_STANDARD 20)

#include platform-specific source files
if(WIN32)
    list(APPEND SOURCES Socket_windows.cpp)
elseif(APPLE)
    list(APPEND SOURCES Socket_posix.cpp)
elseif (UNIX)
    list(APPEND SOURCES Socket_posix.cpp)
endif ()

add_library(Socket STATIC ${SOURCES})

target_include_directories(Socket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
