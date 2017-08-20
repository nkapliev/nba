# TODO Move all build-related tmp crap to `build` dir
# See https://github.com/snikulov/google-test-examples/blob/master/build.cmake
cmake_minimum_required(VERSION 3.7)
project(nba)
set(CMAKE_CXX_STANDARD 14)

# For global includes
set(PROJECT_ROOT ./)
include_directories(${PROJECT_ROOT})

# Googletest library
include(ExternalProject)
set(GTEST_ROOT ${CMAKE_BINARY_DIR}/external)
ExternalProject_Add(googletest
        GIT_REPOSITORY https://github.com/google/googletest
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${GTEST_ROOT}
        )
include_directories(${GTEST_ROOT}/include)
link_directories(${GTEST_ROOT}/lib)

# Tests
set(
        NBA_CONTAINERS_TEST_SOURCE_FILES
        test/test.cpp
        test/containers/unit/stack.cpp
)
add_executable(RunUnitTests ${NBA_CONTAINERS_TEST_SOURCE_FILES})
add_dependencies(RunUnitTests googletest)
target_link_libraries(RunUnitTests gtest gtest_main)

# Register tests in CTest
enable_testing()
add_test(
        NAME RunUnitTests
        COMMAND RunUnitTests
)
