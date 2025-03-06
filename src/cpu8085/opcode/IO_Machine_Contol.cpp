#include "cpu8085.h"
#include "Bus.h"

///////////////////////////////////// Push /////////////////////////// 
uint8_t cpu8085::PUSH_B(){
    write(stkp -1 , b);
    write(stkp - 2, c);
    stkp -= 2;
    return 0;
}

uint8_t cpu8085::PUSH_D(){
    write(stkp -1 , d);
    write(stkp - 2, e);
    stkp -= 2;
    return 0;
}

uint8_t cpu8085::PUSH_H(){
    write(stkp -1 , h);
    write(stkp - 2, l);
    stkp -= 2;
    return 0;
}

uint8_t cpu8085::PUSH_PSW(){
    write(stkp -1 , a);
    write(stkp - 2, status);
    stkp -= 2;
    return 0;
}

///////////////////////////////////////// POP /////////////////////////

uint8_t cpu8085::POP_B(){
    c = read(stkp);
    b = read(stkp+1);
    stkp+=2;
    return 0; 
}
uint8_t cpu8085::POP_D(){
    e = read(stkp);
    d = read(stkp+1);
    stkp+=2;
    return 0; 
}
uint8_t cpu8085::POP_H(){
    l = read(stkp);
    h = read(stkp+1);
    stkp+=2;
    return 0; 
}

uint8_t cpu8085::POP_PSW(){
    status = read(stkp);
    a = read(stkp+1);
    stkp+=2;
    return 0; 
}

///////////////////////////////////////////////////////////

uint8_t cpu8085::XTHL(){
    uint8_t temp = read(stkp);
    write(stkp,l);
    l = temp;

    temp = read(stkp+1);
    write(stkp+1,h);
    h = temp;
    return 0;
}

uint8_t cpu8085::SPHL(){
    stkp = ((uint16_t)h <<8) | l ;
    return 0;
}

///////////////////////////// Control ////////////////////

uint8_t cpu8085::NOP(){
    stop_exe_flag = true;
    return 0;
}

uint8_t cpu8085::HLT(){
    stop_exe_flag = true;
    return 0;
}

uint8_t cpu8085::EI(){ // Dont Implement
    return 0;
}

uint8_t cpu8085::DI(){ // Dont Implement
    return 0;
}

/////////////////////////// Input /Output /////////////////////

// IN 50h ==> a <- device[5050] (Mirrors 8 bits to form 16 bits)
uint8_t cpu8085::IN(){
    uint16_t mapped_addr = ((uint16_t)fetched_low << 8) | fetched_low;
    a = read(mapped_addr);
    return 0;
}
//OUT 50h ==> device[5050] <- a
uint8_t cpu8085::OUT(){
    uint16_t mapped_addr = ((uint16_t)fetched_low << 8) | fetched_low;
    write(mapped_addr,a);
    return 0;
}

uint8_t cpu8085::RIM(){ // Dont Implement
    return 0;
}

uint8_t cpu8085::SIM(){ // Dont Implement
    return 0;
}