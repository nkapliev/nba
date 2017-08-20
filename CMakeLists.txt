# TODO Add test coverage report
# TODO Move all build-related tmp crap to `build` dir
# See https://github.com/snikulov/google-test-examples/blob/master/build.cmake
cmake_minimum_required(VERSION 3.7)
project(nba)
set(CMAKE_CXX_STANDARD 14)

set(PROJECT_NAME nba)
set(PROJECT_TEST_NAME nbaUnitTests)
set(PROJECT_ROOT ./)

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
        test/test.cpp
        test/containers/unit/stack.cpp
)
add_executable(${PROJECT_TEST_NAME} ${TEST_SOURCE_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)
target_link_libraries(${PROJECT_TEST_NAME} gtest gtest_main -lpthread)

# Code coverage
set(CMAKE_MODULE_PATH ${PROJECT_ROOT}/CMakeModules)
#if(CMAKE_COMPILER_IS_GNUCXX)
include(CodeCoverage)
#setup_target_for_coverage(
#        NAME ${PROJECT_NAME}_coverage
#        EXECUTABLE ${PROJECT_TEST_NAME}
#        DEPENDENCIES coverage
#)
##endif()
#
##if(CMAKE_COMPILER_IS_GNUCXX)
#target_link_libraries(${PROJECT_TEST_NAME} gcov -lgcov) # TODO
#link_libraries(gcov)
#endif()

add_executable(${PROJECT_TEST_NAME}-test-coverage ${TEST_SOURCE_FILES})

set_target_properties(
        ${PROJECT_TEST_NAME}-test-coverage PROPERTIES
        COMPILE_FLAGS "-O0 -g -fprofile-arcs -ftest-coverage"
)

set_target_properties(
        ${PROJECT_TEST_NAME}-test-coverage PROPERTIES
        LINK_FLAGS "-O0 -g -lgcov -fprofile-arcs -ftest-coverage"
)

SETUP_TARGET_FOR_COVERAGE(
        NAME coverage
        EXECUTABLE ${PROJECT_TEST_NAME}-test-coverage
        DEPENDENCIES coverage_report
)


# Register tests in CTest
enable_testing()
add_test(
        NAME ${PROJECT_TEST_NAME}
        COMMAND ${PROJECT_TEST_NAME}
)
