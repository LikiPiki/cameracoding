
find_package(GTest REQUIRED)

add_executable(test
    simple.cpp
    )

include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(test ${GTEST_LIBRARIES} pthread)

