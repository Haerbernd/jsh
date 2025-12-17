cmake_minimum_required(VERSION 3.30.5)

project(jsh
        LANGUAGES CXX
        DESCRIPTION "jsh - JanShell my own shell"
        HOMEPAGE_URL "https://jsh.haerbernd.dev"
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src bin)
