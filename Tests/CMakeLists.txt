#cmake version
cmake_minimum_required(VERSION 2.6)

#variables
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

#include headers
include_directories(../Pirus/Header)

#add files
file(GLOB HEADERS "../Pirus/Header/*.h" "../Pirus/Header/*.hpp" "Tests/catch.hpp")
file(GLOB SOURCES "../Pirus/Source/*.cpp" "Tests/*.cpp")

#remove source.cpp from pirus folder
list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../Pirus/Source/Source.cpp)

#make execs
add_executable(pirus_test ${HEADERS} ${SOURCES})
add_test(pirus_test pirus_test)