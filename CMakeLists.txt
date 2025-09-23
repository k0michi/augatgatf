cmake_minimum_required(VERSION 3.10)
project(Main)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

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

# SPIRV-Cross

FetchContent_Declare(
  spirv-cross
  GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross.git
  GIT_TAG vulkan-sdk-1.4.321.0
)
FetchContent_MakeAvailable(spirv-cross)

# main

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

add_library(kl STATIC
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
  src/kl/graphics/opengl/gl_context_descriptor.cc
  src/kl/graphics/program.cc
  src/kl/graphics/rasterization_state.cc
  src/kl/graphics/shader.cc
  src/kl/graphics/swapchain.cc
  src/kl/graphics/texture.cc
  src/kl/platform/frame_awaiter.cc
  src/kl/platform/instance.cc
  src/kl/platform/window.cc
# end
)

target_include_directories(kl PUBLIC include)
target_link_libraries(kl PUBLIC SDL3::SDL3)
target_link_libraries(kl PUBLIC glad)
target_link_libraries(kl PUBLIC spirv-cross-core spirv-cross-glsl)
target_link_libraries(kl PUBLIC coverage_config)

add_executable(main
  src/main.cc
)
target_link_libraries(main PRIVATE kl)

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
add_executable(test_main
# include test/**/*.cc
  test/test_matrix2x2.cc
  test/test_matrix4x4.cc
  test/test_vector2.cc
# end
)
add_test(NAME Tests COMMAND test_main)
target_link_libraries(test_main PRIVATE coverage_config gtest_main kl)

# GLSL compilation

find_program(GLSLANG_VALIDATOR glslangValidator)

set(GLSL_FILES
  shaders/test.vert
)

set(SPIRV_OUTPUTS "")
foreach(GLSL_FILE ${GLSL_FILES})
  get_filename_component(GLSL_BASENAME ${GLSL_FILE} NAME)
  get_filename_component(GLSL_DIR ${GLSL_FILE} DIRECTORY)
  set(GLSL_ABS "${CMAKE_SOURCE_DIR}/${GLSL_FILE}")
  set(SPIRV_OUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${GLSL_DIR}/${GLSL_BASENAME}.spv")

  add_custom_command(
    OUTPUT ${SPIRV_OUT}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${GLSL_DIR}
    COMMAND ${GLSLANG_VALIDATOR} -V ${GLSL_ABS} -o ${SPIRV_OUT}
    DEPENDS ${GLSL_ABS}
    COMMENT "Compiling GLSL to SPIR-V: ${GLSL_FILE}"
  )

  list(APPEND SPIRV_OUTPUTS ${SPIRV_OUT})
endforeach()

add_custom_target(
  shaders ALL
  DEPENDS ${SPIRV_OUTPUTS}
)