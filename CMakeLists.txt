# TODO Move all build-related tmp crap to `build` dir
# See https://github.com/snikulov/google-test-examples/blob/master/build.cmake
cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 14)
set(PROJECT_NAME nba)

set(PROJECT_BUILD_DIR ${CMAKE_BINARY_DIR}/build)

set(build_dir ${PROJECT_BUILD_DIR})

execute_process(
  COMMAND ${CMAKE_COMMAND} .
  WORKING_DIRECTORY ${build_dir}
)

execute_process(
  COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${build_dir}
	RESULT_VARIABLE build_result
)

if(${build_result})
  message(FATAL_ERROR "Build failed")
endif()

execute_process(
  COMMAND make coverage
  WORKING_DIRECTORY ${build_dir}
  RESULT_VARIABLE test_result
)

if(${test_result})
  message(FATAL_ERROR "test failed")
endif()
