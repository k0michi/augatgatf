cmake_minimum_required(VERSION 3.10)
project(Main)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# SDL3 setup

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG release-3.2.22
)
FetchContent_MakeAvailable(SDL3)

add_executable(main
    src/main.cc
    src/kl/platform/window.cc
)
target_include_directories(main PRIVATE include)
target_link_libraries(main PRIVATE SDL3::SDL3)

# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(test_main test/test_main.cc)
target_link_libraries(test_main gtest_main)
add_test(NAME SimpleTests COMMAND test_main)
