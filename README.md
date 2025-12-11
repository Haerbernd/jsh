You need the readline headers installed on your system (e.g. `libreadline-dev` on Ubuntu, `readline` on Arch)

## Compiling with CMake
`cmake -S . -B build`
`cmake --build build`

## Compiling with GCC
1. move all `.h` files in `src/include` to `src`
2. run `g++ src/main.cpp src/builtins.cpp src/completion.cpp src/os.cpp -lreadline`
