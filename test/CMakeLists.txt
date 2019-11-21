cmake_minimum_required (VERSION 3.5)
project(test_linalg)

file(GLOB TEST_SOURCES
    "*.hpp"
    "*.cpp"
)

file(GLOB SOURCES
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.hpp"
)

list(REMOVE_ITEM SOURCES "${SRC_DIR}/main.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${TEST_SOURCES})

add_dependencies(${PROJECT_NAME} linalg)

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC "${THIRDPARTY_DIR}")
