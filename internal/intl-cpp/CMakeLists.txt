cmake_minimum_required(VERSION 3.10)
project(intl-cpp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

SET(SOURCE_FILES
  src/string.cc
)

include(FetchContent)

# icu

FetchContent_Declare(
  icu-portable
  GIT_REPOSITORY https://github.com/k0michi/icu-portable
  GIT_TAG        4695e0c00e16b036595e4e800e32c4b802786dac
)

FetchContent_MakeAvailable(icu-portable)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ICU::data ICU::i18n ICU::io ICU::uc)

# GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(test_intl_cpp
  test/test_string.cc
)
target_link_libraries(test_intl_cpp PRIVATE gtest_main intl-cpp)
add_test(NAME test_intl_cpp COMMAND test_intl_cpp)