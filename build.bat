@echo off

IF NOT "%1" == "-no-cmake" (
    cmake -DCMAKE_BUILD_TYPE=Debug -DLOADER_TYPE=LOADER_GLCORE33 -G Ninja -B ./bin .
)

ninja -C bin
