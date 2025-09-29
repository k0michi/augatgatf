cmake_minimum_required(VERSION 3.20)
project(web-audio-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(web-audio-cpp INTERFACE)
target_include_directories(web-audio-cpp INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

include(FetchContent)

# GTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB WEB_AUDIO_TEST_SRC "test/*.cc")
add_executable(test_web_audio ${WEB_AUDIO_TEST_SRC})
target_link_libraries(test_web_audio PRIVATE web-audio-cpp GTest::gtest_main)
add_test(NAME test_web_audio COMMAND test_web_audio)
