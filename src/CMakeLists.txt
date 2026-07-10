add_executable(jsh ./main.cpp)
add_library(jsh_core SHARED ./builtins.cpp ./os.cpp ./completion.cpp)
target_link_libraries(jsh_core readline)
target_link_libraries(jsh readline jsh_core)

add_library(jsh_random SHARED ./rng.cpp)
