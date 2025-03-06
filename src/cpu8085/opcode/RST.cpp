#include "cpu8085.h"
#include "Bus.h"

uint8_t cpu8085::RST_helper(uint8_t n){
    uint8_t pc_lower = pc & 0xFF;         // Extract lower 8 bits
    uint8_t pc_higher = (pc >> 8) & 0xFF; // Extract higher 8 bits
    write(stkp-1,pc_higher);
    write(stkp-2,pc_lower);
    stkp-=2;
    pc = 0+ 8*n;
    stop_exe_flag = true;
    return 0;
}

////////////////////////////// Software Interrupt ////////////////////////

uint8_t cpu8085::RST_0(){
    return RST_helper(0);
}

uint8_t cpu8085::RST_1(){
    return RST_helper(1);
}

uint8_t cpu8085::RST_2(){
    return RST_helper(2);
}

uint8_t cpu8085::RST_3(){
    return RST_helper(3);
}

uint8_t cpu8085::RST_4(){
    return RST_helper(4);
}

uint8_t cpu8085::RST_5(){
    return RST_helper(5);
}

uint8_t cpu8085::RST_6(){
    return RST_helper(6);
}

uint8_t cpu8085::RST_7(){
    return RST_helper(7);
}

//////////////////////////// Hardware Interrupt [no -opcode] ///////////////////////////

//RST5.5 , RST6.5 , RST7.5 , TRAP , INTR

//// Dont Implement