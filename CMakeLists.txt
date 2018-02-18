cmake_minimum_required(VERSION 3.0.0)
project(cmusclient VERSION 1.0.0)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wno-missing-braces")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(lib)
add_subdirectory(cli)
