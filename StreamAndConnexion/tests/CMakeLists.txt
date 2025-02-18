FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        devel
)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

include(CTest)
include(Catch)

add_executable(tests test.cpp)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain ServerV2lib ClientV2lib)  # Ensure Socket is linked last
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/ClientV2)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/ServerV2)

catch_discover_tests(tests)