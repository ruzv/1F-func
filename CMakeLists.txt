cmake_minimum_required(VERSION 3.14)
project(1F_func)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(
  func
  main.cpp
)

enable_testing()

add_executable(
  func_test
  func_test.cpp
)
target_link_libraries(
  func_test
  gmock
  gtest
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(func_test)
