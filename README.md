# JSH
[![CMake Linux](https://github.com/Haerbernd/jsh/actions/workflows/cmake-single-platform.yml/badge.svg?branch=master)](https://github.com/Haerbernd/jsh/actions/workflows/cmake-single-platform.yml)

A bash-inspired shell created in part through CodeCrafters build-your-own-shell challenge.

## Dependencies
You need the readline headers installed on your system (e.g. `libreadline-dev` on Ubuntu, `readline` on Arch)

### Compiling with CMake
```bash
git clone https://github.com/Haerbernd/jsh.git jsh
cd jsh
cmake -S . -B build
cmake --build build
```

If you also want to run the test:

```bash
ctest --test-dir build --output-on-failure
```

### Compiling with GCC
1. move all `.h` files in `src/include` to `src`
2. run `g++ src/main.cpp src/builtins.cpp src/completion.cpp src/os.cpp -lreadline`

## Roadmap
For now jsh is in a pre-release state. While it works, I want to implement the following functionalities before I make a first release (version 1.0.0.0):

- Appending STDOUT/STDERR to a file (1>\>/2>>)
- Pipelines
- Some missing parts of command history

I plan to add them all before the end of January. When all of the above are implemented jsh will pass all stages of the CodeCrafters build-your-own-shell challenge that inspired/started this project.

When this is achieved I will start adding more tests (mostly unit tests) and documentation. I will then start to think about more features and about the possibility of Windows support.
