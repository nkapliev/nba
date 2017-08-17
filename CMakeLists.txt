cmake_minimum_required(VERSION 3.7)
project(nba)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES test/containers/unit/stack.cpp)
add_executable(nba ${SOURCE_FILES})
