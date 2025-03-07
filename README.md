# 8085_temp
Temporary repo for 8085 emulator
## How to Run 
    sudo apt update
    sudo apt install build-essential libglu1-mesa-dev libpng-dev
    cd 8085_temp
    make -j${nproc}
    ./build/emu <start_address> <input_file> [optional]<mode>

## Additional command for wsl users before running program
    export LIBGL_ALWAYS_SOFTWARE=1
