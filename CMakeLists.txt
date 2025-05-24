cmake_minimum_required(VERSION 3.25)

project(sparz)

# Set the source directory
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Collect all source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${SRC_DIR}/*.cpp)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add warnings to the compilation
add_compile_options(-Wall -Wextra -Wpedantic)

# Set the debug flags
if(CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_options(-O3)
else()
    add_compile_options(-g)
    add_definitions(-DDEBUG_BUILD)
endif()

# Include directories for headers
include_directories(${SRC_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

# Create the executable
add_executable(sparz ${SOURCES})