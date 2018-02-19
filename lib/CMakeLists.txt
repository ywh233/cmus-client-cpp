include(${PROJECT_SOURCE_DIR}/deps/asio.cmake)

add_library(cmusclient
    cmus_client.cc
    cmus_client.h
    connection_interface.h
    ip_connection_interface.cc
    ip_connection_interface.h
    status.cc
    status.h
    unix_connection_interface.cc
    unix_connection_interface.h
)
