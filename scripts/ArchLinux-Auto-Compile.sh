#!/usr/bin/env sh

sudo pacman -Syu --needed git readline cmake

cd ~
git clone https://github.com/Haerbernd/jsh.git jsh
cd jsh

cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure