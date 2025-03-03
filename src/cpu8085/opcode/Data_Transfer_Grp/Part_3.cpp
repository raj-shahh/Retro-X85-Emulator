#include "cpu8085.h"
#include "Bus.h"

/////////////////////////////////////// MVI //////////////////////
uint8_t cpu8085::MVI_A(){
    a = fetched_low;
    return 0;
}

uint8_t cpu8085::MVI_B(){
    b = fetched_low;
    return 0;
}

uint8_t cpu8085::MVI_C(){
    c = fetched_low;
    return 0;
}

uint8_t cpu8085::MVI_D(){
    d = fetched_low;
    return 0;
}

uint8_t cpu8085::MVI_E(){
    e = fetched_low;
    return 0;
}

uint8_t cpu8085::MVI_H(){
    h = fetched_low;
    return 0;
}

uint8_t cpu8085::MVI_L(){
    l = fetched_low;
    return 0;
}
//[H-L]<-fetched_low
uint8_t cpu8085::MVI_M(){
    write(((uint16_t)h << 8) | l , fetched_low);
    return 0;
}

/////////////////////////////////// LXI //////////////////////////////

uint8_t cpu8085::LXI_B(){
    b = fetched_high;
    c = fetched_low;
    return 0;
}

uint8_t cpu8085::LXI_D(){
    d = fetched_high;
    e = fetched_low;
    return 0;
}

uint8_t cpu8085::LXI_H(){
    h = fetched_high;
    l = fetched_low;
    return 0;
}

uint8_t cpu8085::LXI_SP(){
    stkp = ((uint16_t)fetched_high << 8) | fetched_low;
    return 0;
}

//////////////////////////// Load ////////////////////////////

uint8_t cpu8085::LDAX_B(){
    a = read(((uint16_t)b<<8)|c);
    return 0;
}

uint8_t cpu8085::LDAX_D(){
    a = read(((uint16_t)d<<8)|e);
    return 0;
}

uint8_t cpu8085::LHLD(){
    l = read(addr_abs);
    h = read(addr_abs+1);
    return 0;
}

uint8_t cpu8085::LDA(){
    a = read(addr_abs);
    return 0;
}

///////////////////////// Store ///////////////////////////////////////

uint8_t cpu8085::STAX_B(){
    write(((uint16_t)b<<8)|c , a);
    return 0;
}

uint8_t cpu8085::STAX_D(){
    write(((uint16_t)d<<8)|e, a);
    return 0;
}

uint8_t cpu8085::SHLD(){
    write(addr_abs, l);
    write(addr_abs+1 , h);
    return 0;
}

uint8_t cpu8085::STA(){
    write(addr_abs,a);
    return 0;
}