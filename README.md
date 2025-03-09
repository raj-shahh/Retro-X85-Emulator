# 8085_temp
Temporary repo for 8085 emulator

## Dependency Setup
    sudo apt update
    sudo apt install build-essential libglu1-mesa-dev libpng-dev  ;for olcGameEngine
    sudo apt install libgtest-dev cmake g++ make                  ;for gtest (install)
    cd /usr/src/googletest
    sudo cmake . && sudo make && sudo make install                ;build gtest

## How to Build and Run(Release Mode)
    cd 8085_temp
    make -j${nproc}                                             ; build
    ./build/emu 8000 ./egInputs/add2Nums.op --step_wise         ; run

## How to Build and Run(Debug Mode)
    cd 8085_temp
    make clean                                                        ; to del ./build
    make debug -j${nproc}                                             ; build
    gdb --args ./build/emu 8000 ./egInputs/add2Nums.op --one_go       ; run

## How to Build and Run(Tests)
    cd 8085_temp/tests
    make -j${nproc}                 ; builds and then runs
    make clean                      ; to del ../build_tests

## Additional command for wsl users before running program
    export LIBGL_ALWAYS_SOFTWARE=1