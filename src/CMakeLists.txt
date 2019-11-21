cmake_minimum_required (VERSION 3.5)
project(linalg)

file(GLOB SOURCES
    "*.hpp"
    "*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})