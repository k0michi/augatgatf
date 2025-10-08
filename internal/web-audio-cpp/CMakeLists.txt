cmake_minimum_required(VERSION 3.20)
project(web-audio-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(web-audio-cpp STATIC
# include src/**/*.cc
  src/web_audio/audio_buffer.cc
  src/web_audio/audio_context.cc
  src/web_audio/audio_destination_node.cc
  src/web_audio/audio_listener.cc
  src/web_audio/audio_node.cc
  src/web_audio/audio_param.cc
  src/web_audio/audio_render_capacity.cc
  src/web_audio/audio_scheduled_source_node.cc
  src/web_audio/base_audio_context.cc
  src/web_audio/delay_node.cc
  src/web_audio/detail/audio_graph.cc
  src/web_audio/detail/audio_listener_node.cc
  src/web_audio/detail/event_queue.cc
  src/web_audio/detail/message_queue.cc
  src/web_audio/detail/param_collection.cc
  src/web_audio/detail/render_quantum.cc
  src/web_audio/dom_exception.cc
  src/web_audio/offline_audio_context.cc
  src/web_audio/oscillator_node.cc
  src/web_audio/periodic_wave.cc
# end
)
target_include_directories(web-audio-cpp PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

option(WEB_AUDIO_BACKEND_SDL3 "Use SDL3 backend" OFF)

if(WEB_AUDIO_BACKEND_SDL3)
  message(STATUS "web-audio-cpp backend: SDL3")
  
  if(NOT TARGET SDL3::SDL3)
    message(FATAL_ERROR "WEB_AUDIO_BACKEND_SDL3 is ON but SDL3::SDL3 target not found")
  endif()

  target_compile_definitions(web-audio-cpp PUBLIC WEB_AUDIO_BACKEND_SDL3)
  target_link_libraries(web-audio-cpp PUBLIC SDL3::SDL3)
else()
  message(STATUS "web-audio-cpp backend: Null")
  target_compile_definitions(web-audio-cpp PUBLIC WEB_AUDIO_BACKEND_NULL)
endif()

# GTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(WEB_AUDIO_TEST_SRC
# include src/**/*.cc
  src/web_audio/audio_buffer.cc
  src/web_audio/audio_context.cc
  src/web_audio/audio_destination_node.cc
  src/web_audio/audio_listener.cc
  src/web_audio/audio_node.cc
  src/web_audio/audio_param.cc
  src/web_audio/audio_render_capacity.cc
  src/web_audio/audio_scheduled_source_node.cc
  src/web_audio/base_audio_context.cc
  src/web_audio/delay_node.cc
  src/web_audio/detail/audio_graph.cc
  src/web_audio/detail/audio_listener_node.cc
  src/web_audio/detail/event_queue.cc
  src/web_audio/detail/message_queue.cc
  src/web_audio/detail/param_collection.cc
  src/web_audio/detail/render_quantum.cc
  src/web_audio/dom_exception.cc
  src/web_audio/offline_audio_context.cc
  src/web_audio/oscillator_node.cc
  src/web_audio/periodic_wave.cc
# end
# include test/**/*.cc
  test/test_audio_buffer.cc
  test/test_audio_graph.cc
  test/test_audio_node.cc
  test/test_audio_param.cc
  test/test_delay_node.cc
  test/test_message_queue.cc
  test/test_offline_audio_context.cc
  test/test_periodic_wave.cc
  test/test_promise.cc
  test/test_render_quantum.cc
# end
)
add_executable(test_web_audio ${WEB_AUDIO_TEST_SRC})
target_link_libraries(test_web_audio PUBLIC GTest::gtest_main SDL3::SDL3)
add_test(NAME test_web_audio COMMAND test_web_audio)
target_include_directories(test_web_audio PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# https://github.com/codecov/example-cpp11-cmake/blob/master/CMakeLists.txt
add_library(web_audio_coverage_config INTERFACE)

option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(web_audio_coverage_config INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(web_audio_coverage_config INTERFACE --coverage)
  else()
    target_link_libraries(web_audio_coverage_config INTERFACE --coverage)
  endif()
endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

target_compile_definitions(test_web_audio PUBLIC WEB_AUDIO_TEST)
target_link_libraries(test_web_audio PRIVATE web_audio_coverage_config)

add_executable(oscillator example/oscillator.cc)
target_link_libraries(oscillator PRIVATE web-audio-cpp)