#!/bin/bash

if [ "$1" != "-no-cmake" ]
then
    cmake -DCMAKE_BUILD_TYPE=Debug -DLOADER_TYPE=LOADER_GLES3 -G Ninja -B ./bin .
fi

ninja -C bin
