cmake_minimum_required(VERSION 3.5)
project(risc_vector)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -lstdc++)
add_compile_options(-Wextra -Wpedantic)

# cpp standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER "g++")

# header files
include_directories(
	${PROJECT_SOURCE_DIR}/inc
)

# gather source files
file(GLOB_RECURSE SRCS "src/*.cc")
list(REMOVE_ITEM SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc")

# find python3 components
find_package(Python3 COMPONENTS Development REQUIRED)

# binary executable
add_executable(${PROJECT_NAME} ${SRCS} src/main.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE Python3::Python)

find_package(Catch2 REQUIRED)

#gather test files
file(GLOB_RECURSE TESTS "tests/*.cc")

# test executable
add_executable(tests ${SRCS} ${TESTS})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain PRIVATE Python3::Python)

# test discovery
include(CTest)
include(Catch)
catch_discover_tests(tests)
