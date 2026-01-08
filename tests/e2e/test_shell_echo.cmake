if(NOT DEFINED JSH_EXECUTABLE)
        message(FATAL_ERROR "JSH_EXECUTABLE not set")
endif()

message(STATUS "Running shell at ${JSH_EXECUTABLE}")

execute_process(
        COMMAND "${JSH_EXECUTABLE}"
        INPUT_FILE "${CMAKE_CURRENT_LIST_DIR}/input_echo.txt"
        OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/output.txt"
        ERROR_FILE "${CMAKE_CURRENT_BINARY_DIR}/error.txt"
        RESULT_VARIABLE res
)

message(STATUS "execute_process result: ${res}")

if (NOT res EQUAL 0)
        message(FATAL_ERROR "Shell exited with ${res}")
endif()

file(READ "${CMAKE_CURRENT_BINARY_DIR}/output.txt" content)
message(STATUS "Shell output '${content}'")

if(NOT content MATCHES "hello")
        message(FATAL_ERROR "Expected output not found")
endif()
