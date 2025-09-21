cmake_minimum_required(VERSION 3.10)
project(Main)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# SDL3

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG release-3.2.22
)
FetchContent_MakeAvailable(SDL3)

# glad
add_subdirectory(external/glad)

# main

add_executable(main
    src/main.cc
    src/kl/platform/window.cc
	src/kl/platform/instance.cc
	src/kl/platform/frame_awaiter.cc
	src/kl/graphics/instance.cc
	src/kl/graphics/surface.cc
	src/kl/graphics/device.cc
	src/kl/graphics/opengl/gl_context.cc
	src/kl/graphics/context.cc
)
target_include_directories(main PRIVATE include)
target_link_libraries(main PRIVATE SDL3::SDL3)
target_link_libraries(main PRIVATE glad)

if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX .html)
endif()

# GoogleTest
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
