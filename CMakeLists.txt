# TODO Move all build-related tmp crap to `build` dir
# See https://github.com/snikulov/google-test-examples/blob/master/build.cmake
cmake_minimum_required(VERSION 3.7)
project(nba)
set(CMAKE_CXX_STANDARD 14)

include(ExternalProject)
set(GTEST_ROOT ${CMAKE_BINARY_DIR}/external)
ExternalProject_Add(googletest
        GIT_REPOSITORY https://github.com/google/googletest
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${GTEST_ROOT}
        )
include_directories(${GTEST_ROOT}/include)
link_directories(${GTEST_ROOT}/lib)

set(PROJECT_ROOT ./)
include_directories(${PROJECT_ROOT})

set(TEST_SOURCE_FILES test/test.cpp test/containers/unit/stack.cpp)


enable_testing()
find_package(GTest REQUIRED)

cmake_policy(SET CMP0048 NEW)
project(my_gtest_pkgconfig VERSION 0.0.1 LANGUAGES CXX)

find_package(PkgConfig)
pkg_search_module(GTEST REQUIRED gtest_main)

add_executable(nba_test ${TEST_SOURCE_FILES})
add_dependencies(nba_test googletest)

target_link_libraries(nba_test gtest gtest_main ${GTEST_LDFLAGS})
target_compile_options(nba_test PUBLIC ${GTEST_CFLAGS})

# TODO Integrate with CTest
#include(CTest)
#add_test(first_and_only_test nba_test)
