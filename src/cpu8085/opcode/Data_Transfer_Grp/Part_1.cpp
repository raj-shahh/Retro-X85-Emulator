#include "cpu8085.h"
#include "Bus.h"

/////////////////////////////////////// Mov A , r/M ////////////////////////////
uint8_t cpu8085::MOV_A_A(){
    a = a ;
    return 0;
}

uint8_t cpu8085::MOV_A_B(){
    a = b ;
    return 0;
}

uint8_t cpu8085::MOV_A_C(){
    a = c ;
    return 0;
}

uint8_t cpu8085::MOV_A_D(){
    a = d ;
    return 0;
}

uint8_t cpu8085::MOV_A_E(){
    a = e ;
    return 0;
}

uint8_t cpu8085::MOV_A_H(){
    a = h ;
    return 0;
}

uint8_t cpu8085::MOV_A_L(){
    a = l ;
    return 0;
}

// A <- [H-L]
uint8_t cpu8085::MOV_A_M(){
    a = read(((uint16_t)h << 8) | l);
    return 0;
}

////////////////////////////////// Mov B , r/M //////////////////////////////////////

uint8_t cpu8085::MOV_B_A(){
    b = a ;
    return 0;
}

uint8_t cpu8085::MOV_B_B(){
    b = b ;
    return 0;
}

uint8_t cpu8085::MOV_B_C(){
    b = c ;
    return 0;
}

uint8_t cpu8085::MOV_B_D(){
    b = d ;
    return 0;
}

uint8_t cpu8085::MOV_B_E(){
    b = e ;
    return 0;
}

uint8_t cpu8085::MOV_B_H(){
    b = h ;
    return 0;
}

uint8_t cpu8085::MOV_B_L(){
    b = l ;
    return 0;
}

// b <- [H-L]
uint8_t cpu8085::MOV_B_M(){
    b = read(((uint16_t)h << 8) | l);
    return 0;
}

////////////////////////// Mov C , r /M ///////////////////////////////////////////////

uint8_t cpu8085::MOV_C_A(){
    c = a ;
    return 0;
}

uint8_t cpu8085::MOV_C_B(){
    c = b ;
    return 0;
}

uint8_t cpu8085::MOV_A_C(){
    c = c ;
    return 0;
}

uint8_t cpu8085::MOV_C_D(){
    c = d ;
    return 0;
}

uint8_t cpu8085::MOV_C_E(){
    c = e ;
    return 0;
}

uint8_t cpu8085::MOV_C_H(){
    c = h ;
    return 0;
}

uint8_t cpu8085::MOV_C_L(){
    c = l ;
    return 0;
}

// C <- [H-L]
uint8_t cpu8085::MOV_C_M(){
    c = read(((uint16_t)h << 8) | l);
    return 0;
}

//////////////// Mov D , r/M /////////////////////////////////////////////////////////

uint8_t cpu8085::MOV_D_A(){
    d = a ;
    return 0;
}

uint8_t cpu8085::MOV_D_B(){
    d = b ;
    return 0;
}

uint8_t cpu8085::MOV_D_C(){
    d = c ;
    return 0;
}

uint8_t cpu8085::MOV_D_D(){
    d = d ;
    return 0;
}

uint8_t cpu8085::MOV_D_E(){
    d = e ;
    return 0;
}

uint8_t cpu8085::MOV_D_H(){
    d = h ;
    return 0;
}

uint8_t cpu8085::MOV_D_L(){
    d = l ;
    return 0;
}

// D <- [H-L]
uint8_t cpu8085::MOV_D_M(){
    d = read(((uint16_t)h << 8) | l);
    return 0;
}

