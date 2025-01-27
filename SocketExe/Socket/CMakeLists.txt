cmake_minimum_required(VERSION 3.29)
project(Socket)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES Socket_common.cpp)

#include platform-specific source files
if(WIN32)
    list(APPEND SOURCES Socket_windows.cpp)
elseif(APPLE)
    list(APPEND SOURCES Socket_posix.cpp)
elseif (UNIX)
    list(APPEND SOURCES Socket_posix.cpp)
endif ()

add_library(Socket ${SOURCES})
