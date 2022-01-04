cmake_minimum_required(VERSION 3.10)

project(cameracoding C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(${CMAKE_CURRENT_LIST_DIR}/src/src.cmake)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test)

add_executable(${PROJECT_NAME} ${SRC} ${CMAKE_CURRENT_LIST_DIR}/src/main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE m)
