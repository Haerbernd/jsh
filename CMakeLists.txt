cmake_minimum_required(VERSION 3.30.5)

project(jsh
        LANGUAGES CXX
        DESCRIPTION "jsh - JanShell my own shell"
        VERSION 1.0.0.0
        HOMEPAGE_URL "https://jsh.haerbernd.dev"
)

include(CTest)
include(GNUInstallDirs)

set(CPACK_PACKAGE_NAME "jsh")
set(CPACK_PACKAGE_VENDOR "Jan-Hendrik Schmidt")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_SOURCE_DIR}/packaging)
set(CPACK_PACKAGE_HOMEPAGE_URL ${HOMEPAGE_URL})
set(CPACK_PACKAGE_CHECKSUM SHA256)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)

set(CPACK_GENERATOR TGZ STGZ ZIP)

if(BUILD_TESTING)
        add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/common-tools-library ${CMAKE_BINARY_DIR}/libs)
include_directories(${CMAKE_SOURCE_DIR}/external/common-tools-library/src/include)

include_directories(${CMAKE_SOURCE_DIR}/src/include)
add_subdirectory(${CMAKE_SOURCE_DIR}/internal/jsh_core ${CMAKE_BINARY_DIR}/libs/jsh_core)

add_subdirectory(${CMAKE_SOURCE_DIR}/src ${CMAKE_BINARY_DIR}/bin)

install(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION .)

include(CPack) # Must always be after all CPACK_ Variables are defined
