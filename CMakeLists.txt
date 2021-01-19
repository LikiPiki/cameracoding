cmake_minimum_required(VERSION 3.13)

project(coding C CXX)

# set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# enable_testing()

set(Sources
    main.c
    )

add_subdirectory(tests)

add_executable(${PROJECT_NAME} ${Sources})

# link with math library
target_link_libraries(${PROJECT_NAME} m)


