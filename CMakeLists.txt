cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_COMPILER "g++")
project(risc_vector)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -lstdc++ -g -O0)
add_compile_options(-Wextra -Wpedantic)

# cpp standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# header files
include_directories(
	${PROJECT_SOURCE_DIR}/inc
)

# add gui
add_subdirectory(gui)

# gather source files
file(GLOB_RECURSE SRCS "src/*.cc")

# find QT components
find_package(Qt6 COMPONENTS Widgets REQUIRED)
qt_standard_project_setup()

# binary executable
add_library(${PROJECT_NAME}_lib ${SRCS})
target_link_libraries(${PROJECT_NAME}_lib)

find_package(Catch2 REQUIRED)

#gather test files
file(GLOB_RECURSE TESTS "tests/*.cc")

# test executable
add_executable(tests ${SRCS} ${TESTS})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain PRIVATE)

# test discovery
include(CTest)
include(Catch)
catch_discover_tests(tests)
