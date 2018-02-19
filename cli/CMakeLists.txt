include(${PROJECT_SOURCE_DIR}/deps/asio.cmake)

add_executable(status status.cc)
target_link_libraries(status
    cmusclient
    cxxopts)
