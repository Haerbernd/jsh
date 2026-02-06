#!/usr/bin/env sh

sudo apt-get update
sudo apt-get install build-essential libreadline-dev git cmake

cd ~
git clone https://github.com/Haerbernd/jsh.git jsh
cd jsh

cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure