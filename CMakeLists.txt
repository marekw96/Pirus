cmake_minimum_required(VERSION 3.1)

# Solution name
PROJECT(Pirus)

#STD++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable tests
add_subdirectory(Tests)
enable_testing()

# C++ Version
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)

# Include Directories
include_directories(Pirus/Header)

# Add files
file(GLOB_RECURSE HEADERS "Pirus/Header/*.h")
file(GLOB_RECURSE SOURCES "Pirus/Source/*.cpp")

# Create .exe
add_executable(pirus ${HEADERS} ${SOURCES})

