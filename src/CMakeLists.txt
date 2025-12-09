include_directories(${CMAKE_SOURCE_DIR}/src/include)

add_executable(jsh ./main.cpp ./builtins.cpp ./os.cpp)
