cmake_minimum_required (VERSION 3.5)

# Built-in
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Directories
set(THIRDPARTY_DIR "${PROJECT_SOURCE_DIR}/thirdparty")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(TEST_DIR "${PROJECT_SOURCE_DIR}/test")

# Compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O3")

add_subdirectory (${SRC_DIR})
add_subdirectory(${TEST_DIR})