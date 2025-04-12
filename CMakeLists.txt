cmake_minimum_required(VERSION 3.5)
project(ram)

option(RAM_TESTS "Enable creation of a memory-subsystem test binary." ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -lstdc++ -g -O0)
add_compile_options(-Wextra -Wpedantic)

# cpp standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# gather source files
file(GLOB_RECURSE SRCS "src/*.cc")

# binary executable
add_library(${PROJECT_NAME}_lib ${SRCS})
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${PROJECT_SOURCE_DIR}/inc)

if(RAM_TESTS)
	find_package(Catch2 REQUIRED)

	#gather test files
	file(GLOB_RECURSE TESTS "tests/*.cc")

	# test executable
	add_executable(tests ${SRCS} ${TESTS})
	target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR}/inc)
	target_link_libraries(tests PRIVATE Catch2::Catch2WithMain PRIVATE)

	# test discovery
	include(CTest)
	include(Catch)
	catch_discover_tests(tests)
endif()
