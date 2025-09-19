cmake_minimum_required(VERSION 3.10)
project(Main)

add_executable(main src/main.cc)

# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(test_main test/test_main.cc)
target_link_libraries(test_main gtest_main)
add_test(NAME SimpleTests COMMAND test_main)
