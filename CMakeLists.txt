cmake_minimum_required(VERSION 3.0.0)
project(cmusclient VERSION 1.0.0)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wuninitialized -Wunused -Wno-missing-braces -Wno-shorten-64-to-32")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include_directories(${PROJECT_SOURCE_DIR})

enable_testing()

add_subdirectory(deps/cxxopts)
add_subdirectory(deps/gtest)
add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(cli)
