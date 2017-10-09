cmake_minimum_required(VERSION 2.6)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

file(GLOB_RECURSE HEADERS "../Pirus/Header/*.h")
file(GLOB_RECURSE HEADERS "../Pirus/Header/*.hpp")
file(GLOB_RECURSE HEADERS "Tests/catch.hpp")
file(GLOB_RECURSE SOURCES "../Pirus/Source/*.cpp")
file(GLOB_RECURSE SOURCES "Tests/*.cpp")


add_executable(pirus_test ${HEADERS} ${SOURCES})
add_test(pirus_test pirus_test)