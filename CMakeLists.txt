cmake_minimum_required(VERSION 3.5)
project(risc_vector)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOUIC ON) # automatically process .ui files into headers
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

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

# find QT components
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# binary executable
add_executable(${PROJECT_NAME} ${SRCS})
qt_add_resources(GUI_RESOURCES "src/gui/resources.qrc")
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

# find_package(Catch2 REQUIRED)

# #gather test files
# file(GLOB_RECURSE TESTS "tests/*.cc")

# # test executable
# add_executable(tests ${SRCS} ${TESTS})
# target_link_libraries(tests PRIVATE Catch2::Catch2WithMain PRIVATE Qt6::Widgets)

# # test discovery
# include(CTest)
# include(Catch)
# catch_discover_tests(tests)


# # Gather GUI Source Files
# file(GLOB_RECURSE GUI_SRCS "src/gui/*.cc")

# # Gather GUI Resources
# qt_add_resources(GUI_RESOURCES "src/gui/resources.qrc")

# # GUI executable
# qt_add_executable("${PROJECT_NAME}_gui" ${GUI_SRCS} ${SRCS} ${GUI_RESOURCES})
# target_link_libraries("${PROJECT_NAME}_gui" PRIVATE Qt6::Widgets)
