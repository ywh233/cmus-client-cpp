add_executable(lib_test
    main.cc)
target_link_libraries(lib_test
    cmusclient
    gtest)
add_test(NAME lib_test COMMAND lib_test)
