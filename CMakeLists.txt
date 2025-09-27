cmake_minimum_required(VERSION 3.10)
project(main)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_subdirectory(internal/keel)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# main

add_executable(main
  src/main.cc
)
target_link_libraries(main PRIVATE keel)
set_property(TARGET main PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX .html)
endif()

# GLSL compilation

find_program(GLSLANG_VALIDATOR glslangValidator)

set(GLSL_FILES
  shaders/hello.vert
  shaders/hello.frag
  shaders/uniform.vert
  shaders/uniform.frag
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