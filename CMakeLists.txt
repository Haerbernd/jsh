cmake_minimum_required(VERSION 3.31)

project(jsh
        LANGUAGES CXX
        DESCRIPTION "jsh - JanShell my own shell"
)

set(CMAKE_CXX_STANDARD 20) # Enable the C++23 standard

add_subdirectory(src bin)
