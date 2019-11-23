cmake_minimum_required (VERSION 3.5)
project(model_converter)

file(GLOB SOURCES
    "main.cpp"
    "**/*.hpp"
    "**/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})