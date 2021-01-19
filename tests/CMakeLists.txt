
find_package(GTest REQUIRED)

add_executable(testing
	simple.cpp
	)

include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(testing ${GTEST_LIBRARIES} pthread)

