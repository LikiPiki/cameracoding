cmake_minimum_required(VERSION 3.10)

project(tests C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(${CMAKE_SOURCE_DIR}/src/src.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/src/src.cmake)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC} ${SRC_TEST})

target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread m)

