cmake_minimum_required(VERSION 3.30.5)

project(jsh
        LANGUAGES CXX
        DESCRIPTION "jsh - JanShell my own shell"
        VERSION 1.0.0.0
        HOMEPAGE_URL "https://jsh.haerbernd.dev"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# file(COPY ${CMAKE_BINARY_DIR}/compile_commands.json DESTINATION ${CMAKE_SOURCE_DIR}) # Not needed as clangd can find it anyway

include_directories(${CMAKE_SOURCE_DIR}/src/include)

add_subdirectory(src bin)
add_subdirectory(tests)

enable_testing()
