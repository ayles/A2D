#!/usr/bin/env bash
cmake -DCMAKE_TOOLCHAIN_FILE=./ios.cmake -DIOS_PLATFORM=SIMULATOR -H. -Bcmake-build-release -GXcode &&
cmake --build cmake-build-release/ --config Release
