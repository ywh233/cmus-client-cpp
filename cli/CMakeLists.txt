include(${PROJECT_SOURCE_DIR}/deps/asio.cmake)

add_executable(status status.cc)
target_link_libraries(status
    cmusclient
    cxxopts)

add_executable(print_list print_list.cc)
target_link_libraries(print_list
    cmusclient
    cxxopts)
