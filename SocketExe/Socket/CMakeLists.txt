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
endif()

list(APPEND SOURCES Socket_common.cpp)

# Add the Socket library
add_library(Socket STATIC ${SOURCES})

# Make sure the include directories are set
target_include_directories(Socket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})