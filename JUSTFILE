build:
        cmake --build build

build-without-test: setup-cmake-without-test
        cmake --build build

setup-cmake:
        cmake -S . -B build

setup-cmake-without-test:
        cmake -S . -B build -DBUILD_TESTING=OFF

package:
        mkdir packaging
        cpack --config ./build/CPackConfig.cmake
        cpack --config ./build/CPackSourceConfig.cmake
