cmake_minimum_required(VERSION 3.20)
project(keel LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(FetchContent)

find_package(Git)

function(get_version)
if(GIT_FOUND)
    execute_process(
      COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_DESCRIBE_OUT
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET
    )
  endif()

  if("${GIT_DESCRIBE_OUT}" STREQUAL "")
    set(GIT_DESCRIBE_OUT "v0.0.0-unknown")
  endif()

  if(GIT_DESCRIBE_OUT MATCHES "^v?([0-9]+)\\.([0-9]+)\\.([0-9]+)")
    set(ver_major ${CMAKE_MATCH_1})
    set(ver_minor ${CMAKE_MATCH_2})
    set(ver_patch ${CMAKE_MATCH_3})
  else()
    set(ver_major 0)
    set(ver_minor 0)
    set(ver_patch 0)
  endif()

  set(PROJECT_VERSION_MAJOR ${ver_major} PARENT_SCOPE)
  set(PROJECT_VERSION_MINOR ${ver_minor} PARENT_SCOPE)
  set(PROJECT_VERSION_PATCH ${ver_patch} PARENT_SCOPE)
  set(KL_REVISION ${GIT_DESCRIBE_OUT} PARENT_SCOPE)
endfunction()

get_version()

message(STATUS "Keel version: ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
message(STATUS "Keel revision: ${KL_REVISION}")

# SDL3

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.22
)
FetchContent_MakeAvailable(SDL3)

# web-audio-cpp

set(WEB_AUDIO_BACKEND_SDL3 ON CACHE BOOL "Enable SDL3 backend for web-audio-cpp")
add_subdirectory(../web-audio-cpp ${CMAKE_CURRENT_BINARY_DIR}/web-audio-cpp)

# glad

add_subdirectory(../../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)

# SPIRV-Cross

FetchContent_Declare(
  spirv-cross
  GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross.git
  GIT_TAG vulkan-sdk-1.4.321.0
)
FetchContent_MakeAvailable(spirv-cross)

# libuv

if(NOT EMSCRIPTEN)
  FetchContent_Declare(
    libuv
    GIT_REPOSITORY https://github.com/libuv/libuv.git
    GIT_TAG v1.51.0
  )
  FetchContent_MakeAvailable(libuv)
endif()

# stb

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        f1c79c02822848a9bed4315b12c8c8f3761e1296
)

FetchContent_MakeAvailable(stb)

# fmt

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        12.1.0
)

FetchContent_MakeAvailable(fmt)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${stb_SOURCE_DIR}")

# https://github.com/codecov/example-cpp11-cmake/blob/master/CMakeLists.txt
add_library(coverage_config INTERFACE)

option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(coverage_config INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(coverage_config INTERFACE --coverage)
  else()
    target_link_libraries(coverage_config INTERFACE --coverage)
  endif()
endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

# keel

add_library(keel STATIC
# include src/kl/**/*.cc
  src/kl/graphics/buffer.cc
  src/kl/graphics/color_blend_state.cc
  src/kl/graphics/context.cc
  src/kl/graphics/depth_stencil_state.cc
  src/kl/graphics/device.cc
  src/kl/graphics/device_child.cc
  src/kl/graphics/framebuffer.cc
  src/kl/graphics/instance.cc
  src/kl/graphics/instance_child.cc
  src/kl/graphics/opengl/gl_context.cc
  src/kl/graphics/program.cc
  src/kl/graphics/rasterization_state.cc
  src/kl/graphics/sampler.cc
  src/kl/graphics/shader.cc
  src/kl/graphics/swapchain.cc
  src/kl/graphics/texture.cc
  src/kl/graphics/vertex_input_state.cc
  src/kl/io/file.cc
  src/kl/io/file_system.cc
  src/kl/io/loop.cc
  src/kl/io/memory_stream.cc
  src/kl/io/stderr.cc
  src/kl/io/stdin.cc
  src/kl/io/stdout.cc
  src/kl/platform/frame_awaiter.cc
  src/kl/platform/instance.cc
  src/kl/platform/keyboard_state.cc
  src/kl/platform/window.cc
  src/kl/version.cc
# end
)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/include/kl/version.hh.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/kl/version.hh"
  @ONLY
)

target_include_directories(keel PUBLIC include "${CMAKE_CURRENT_BINARY_DIR}/include")
target_link_libraries(keel PUBLIC web-audio-cpp)
target_link_libraries(keel PUBLIC SDL3::SDL3)
target_link_libraries(keel PUBLIC glad)
target_link_libraries(keel PUBLIC spirv-cross-core spirv-cross-glsl)
target_link_libraries(keel PUBLIC stb)
target_link_libraries(keel PUBLIC fmt)
target_link_libraries(keel PUBLIC coverage_config)

if(EMSCRIPTEN)
  target_compile_options(keel PRIVATE -fexceptions -g)
  target_link_options(keel INTERFACE -sFULL_ES3=1 -fexceptions -g -sFETCH)
else()
  target_link_libraries(keel PUBLIC uv)
endif()

# GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_keel
# include test/**/*.cc
  test/test_colors.cc
  test/test_file.cc
  test/test_loop.cc
  test/test_matrix2x2.cc
  test/test_matrix4x4.cc
  test/test_memory_stream.cc
  test/test_png.cc
  test/test_stream_util.cc
  test/test_task.cc
  test/test_vector2.cc
# end
)

add_test(NAME test_keel COMMAND test_keel)
target_link_libraries(test_keel PRIVATE coverage_config gtest_main keel)
set_property(TARGET test_keel PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/test/test_data" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")