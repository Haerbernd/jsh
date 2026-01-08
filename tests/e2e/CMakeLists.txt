add_test(
        NAME shell_echo
        COMMAND ${CMAKE_COMMAND}
        -DJSH_EXECUTABLE=$<TARGET_FILE:jsh>
        -P ${CMAKE_CURRENT_SOURCE_DIR}/test_shell_echo.cmake
)
