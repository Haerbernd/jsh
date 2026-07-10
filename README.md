# JSH
[![Ubuntu (CMake)](https://github.com/Haerbernd/jsh/actions/workflows/ubuntu-cmake.yml/badge.svg)](https://github.com/Haerbernd/jsh/actions/workflows/ubuntu-cmake.yml)

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
```bash
g++ src/main.cpp src/builtins.cpp src/completion.cpp src/os.cpp -lreadline -Isrc/include
```
