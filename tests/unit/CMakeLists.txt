add_executable(test_tokenize test_tokenize.cpp)
target_link_libraries(test_tokenize jsh_core)

add_test(NAME tokenize COMMAND test_tokenize)
