# Intel - 8085A Emulator with Retro Style Dark Themed UI

## Features 
    1. Dual Mode of Input - file.asm - file.op
            a> Inbuilt Assembler with Label Parsing Support (Parses .asm files and generate opcode file to fed as input to emulator)
            b> Machine Code [opcode] format -file.op
    
    2. Dual Mode of Execution --step_wise and --one_go
            a> --step_wise : Debug Mode :- Press SpaceBar to execute Current Instruction
            b> --one_go : Stops execution only on Reaching HLT / NOP
    
    3. Configurable Software Interrupts (RST 0-7) [Optional]
            - Full power in hands of user Can decide where to keep his RST code in memory through a specific FileName format and keeping all 
              Rst codes in a specific folder and passing that folder at command line to emulator
              Eg: - File RST_0_5089.asm has code for RST_0 to be stored at location 5089h.
              Ie. Emu already keeps a JMP statement at 0x0000 for location 5089h as RST_n writes pc on stack and jmps to [0 + 8*n]
    
    4. Dark Themed Retro Style UI renderd using olcPixelGameEngine from ONE_LONE_CODER
    
    5. Hardware Reset Facility Using the R key [Ps changes mode of execution to --step_wise]
            
    6. Examine Memory -> Given Any Valid Address of Entire Address Space allows us to view Contents of Entire Next 256 Addresses at once
                        -- Thus with this facility can view the contents of Entire Address space 256 Address at a time
    
    7. Circular Rollback View of Disassembled Code spanning across current Program Counter

    8. ALL valid opcodes of 8085A is implemented except (RIM ,SIM ,EI ,DI)

    9. View of ALL the Internal Registers and Flags along With 256B Address Space starting from our Program Address

    10. Extensive Test Coverage using gtest framework for cpu opcodes.


## DEMO :-

Check out this demo video:

<a href="https://youtu.be/NqIcutpst98">
  <img src="https://img.youtube.com/vi/NqIcutpst98/maxresdefault.jpg" alt="Watch the video" width="800">
</a>


# ---------------- Building And Runing CodeBase ---------------------
## Dependency Setup
    sudo apt update
    sudo apt install build-essential libglu1-mesa-dev libpng-dev  ;for olcGameEngine
    sudo apt install libgtest-dev cmake g++ make                  ;for gtest (install)
    cd /usr/src/googletest
    sudo cmake . && sudo make && sudo make install                ;build gtest

## How to Build (Release Mode)
    cd Retro-X85-Emulator
    make -j${nproc}                                             ; build

## How to Build and Run(Debug Mode)
    cd Retro-X85-Emulator
    make clean                                                        ; to del ./build
    make debug -j${nproc}                                             ; build
    gdb --args ./build/emu 8000 ./egInputs/add2Nums.op --step_wise    ; run Example

## How to Build and Run(Tests)
    cd Retro-X85-Emulator/tests
    make -j${nproc}                 ; builds and then runs
    make clean                      ; to del ../build_tests

## Additional command for wsl users before running program
    export LIBGL_ALWAYS_SOFTWARE=1

## How to Run(ALL types of Variants)
    Usage <must> [optional]:~$ ./build/emu <prog start_address> <prog filename> <--step_wise> o/r <--one_go> [config RST folder]


# --------------------------- Debian Packaging -----------------------

##  Re-Build Deb Pkg:-
    //After making a new exe ./build/emu
    cd Retro-X85-Emulator
    mv ./build/emu ./Deb_pkg/Retro_X85_Emu/Emu_x85/bin/
    cd ./Deb_pkg; 
    dpkg-deb --build Retro_X85_Emu  // rebuild pkg 

## Install & Uninstall the .deb Package :-
    sudo dpkg -i Retro_X85_Emu.deb // install
    sudo dpkg -r retro-x85-emu     // remove

## Verify Succesfull Install of Deb pkg (check below dir structure exists):-
    /Emu_x85/
    ├── bin/
    │   └── emu
    ├── Config_Rsts/
    │   └── RST_0_1000.asm
    └── egInputs/
        ├── demo1.asm
        └── demo2.op

## Eg Use of the installed deb_pkg :-
    cd /Emu_x85
    ./bin/emu ->Enter to see usage instrction
    // Some example Runs (either do 1> or 2> )->
    1> ./bin/emu 8000 ./egInputs/demo1.asm --one_go ./Config_Rsts
    2> ./bin/emu 1000 ./egInputs/demo2.op --step_wise
