add_executable(lib_test
    cmus_client_unittests.cc
    main.cc)
target_link_libraries(lib_test
    cmusclient
    gmock
    gtest)
add_test(NAME lib_test COMMAND lib_test)
