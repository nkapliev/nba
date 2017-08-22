# TODO Move all build-related tmp crap to `build` dir
# See https://github.com/snikulov/google-test-examples/blob/master/build.cmake
cmake_minimum_required(VERSION 3.7)
project(nba)
set(CMAKE_CXX_STANDARD 14)

set(PROJECT_NAME nba)
set(PROJECT_TEST_NAME nbaUnitTests)
set(PROJECT_ROOT ../)

# For global includes
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
        TEST_SOURCE_FILES
        ${PROJECT_ROOT}/test/test.cpp
        ${PROJECT_ROOT}/test/containers/unit/stack.cpp
        ${PROJECT_ROOT}/test/containers/unit/NBStack.cpp
)
add_executable(${PROJECT_TEST_NAME} ${TEST_SOURCE_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)
target_link_libraries(${PROJECT_TEST_NAME} gtest gtest_main -lpthread)

# Code coverage
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/CMakeModules)
include(CodeCoverage)

add_executable(${PROJECT_TEST_NAME}-test-coverage ${TEST_SOURCE_FILES})
add_dependencies(${PROJECT_TEST_NAME}-test-coverage googletest)
target_link_libraries(${PROJECT_TEST_NAME}-test-coverage gtest gtest_main -lpthread)

set_target_properties(
        ${PROJECT_TEST_NAME}-test-coverage PROPERTIES
        COMPILE_FLAGS "-O0 -g -fprofile-arcs -ftest-coverage"
)

set_target_properties(
        ${PROJECT_TEST_NAME}-test-coverage PROPERTIES
        LINK_FLAGS "-O0 -g -fprofile-arcs -ftest-coverage"
)

set(COVERAGE_EXCLUDES ${TEST_SOURCE_FILES}) # TODO How to exclude /external files?

SETUP_TARGET_FOR_COVERAGE(
        NAME coverage
        EXECUTABLE ${PROJECT_TEST_NAME}-test-coverage
)

# Register tests in CTest
enable_testing()
add_test(
        NAME ${PROJECT_TEST_NAME}
        COMMAND ${PROJECT_TEST_NAME}
)
