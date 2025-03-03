#include "cpu8085.h"
#include "Bus.h"

//////////////// Mov E , r/M ///////////////////////////////////////////////////

uint8_t cpu8085::MOV_E_A(){
    e = a ;
    return 0;
}

uint8_t cpu8085::MOV_E_B(){
    e = b ;
    return 0;
}

uint8_t cpu8085::MOV_E_C(){
    e = c ;
    return 0;
}

uint8_t cpu8085::MOV_E_D(){
    e = d ;
    return 0;
}

uint8_t cpu8085::MOV_E_E(){
    e = e ;
    return 0;
}

uint8_t cpu8085::MOV_E_H(){
    e = h ;
    return 0;
}

uint8_t cpu8085::MOV_E_L(){
    e = l ;
    return 0;
}

// E <- [H-L]
uint8_t cpu8085::MOV_E_M(){
    e = read(((uint16_t)h << 8) | l);
    return 0;
}

////////////// Mov H , r/M //////////////////////////////////////////

uint8_t cpu8085::MOV_H_A(){
    h = a ;
    return 0;
}

uint8_t cpu8085::MOV_H_B(){
    h = b ;
    return 0;
}

uint8_t cpu8085::MOV_H_C(){
    h = c ;
    return 0;
}

uint8_t cpu8085::MOV_H_D(){
    h = d ;
    return 0;
}

uint8_t cpu8085::MOV_H_E(){
    h = e ;
    return 0;
}

uint8_t cpu8085::MOV_H_H(){
    h = h ;
    return 0;
}

uint8_t cpu8085::MOV_H_L(){
    h = l ;
    return 0;
}

// h <- [H-L]
uint8_t cpu8085::MOV_H_M(){
    h = read(((uint16_t)h << 8) | l);
    return 0;
}

//////////////// Mov L , r/M ///////////////////////////////////////////////////

uint8_t cpu8085::MOV_L_A(){
    l = a ;
    return 0;
}

uint8_t cpu8085::MOV_L_B(){
    l = b ;
    return 0;
}

uint8_t cpu8085::MOV_L_C(){
    l = c ;
    return 0;
}

uint8_t cpu8085::MOV_L_D(){
    l = d ;
    return 0;
}

uint8_t cpu8085::MOV_L_E(){
    l = e ;
    return 0;
}

uint8_t cpu8085::MOV_L_H(){
    l = h ;
    return 0;
}

uint8_t cpu8085::MOV_L_L(){
    l = l ;
    return 0;
}

// l <- [H-L]
uint8_t cpu8085::MOV_L_M(){
    l = read(((uint16_t)h << 8) | l);
    return 0;
}


/////////////////////// Mov M , r/M ////////////////////////////////

//[H-L] <- A
uint8_t cpu8085::MOV_M_A(){
    write(((uint16_t)h << 8) | l , a);
    return 0;
}

//[H-L] <- B
uint8_t cpu8085::MOV_M_B(){
    write(((uint16_t)h << 8) | l , b);
    return 0;
}

//[H-L] <- C
uint8_t cpu8085::MOV_M_C(){
    write(((uint16_t)h << 8) | l , c);
    return 0;
}

//[H-L] <- D
uint8_t cpu8085::MOV_M_D(){
    write(((uint16_t)h << 8) | l , d);
    return 0;
}

//[H-L] <- E
uint8_t cpu8085::MOV_M_E(){
    write(((uint16_t)h << 8) | l , e);
    return 0;
}

//[H-L] <- H
uint8_t cpu8085::MOV_M_H(){
    write(((uint16_t)h << 8) | l , h);
    return 0;
}

//[H-L] <- L
uint8_t cpu8085::MOV_M_L(){
    write(((uint16_t)h << 8) | l , l);
    return 0;
}

///////////////////////////////////////////////////////
// swap(d,h) and swap(e,l)
uint8_t cpu8085::XCHG(){
    uint8_t temp = d;
    d= h;
    h= temp;

    temp = e;
    e = l;
    l = temp;

    return 0;
}