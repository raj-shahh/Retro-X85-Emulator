#include "cpu8085.h"
#include "Bus.h"

uint8_t inrDcrReg(cpu8085 &cpu8085, uint8_t reg, bool dcr = false) {
    bool aux_flag_cond = (reg & 0x0F + ((-1) ^ dcr) & 0x0F) > 0x0F;
    cpu8085.allSetFlags(reg + ((-1) ^ dcr) & 0xFF, aux_flag_cond, false);
    reg = (reg + (dcr ? -1 : 1)) & 0xFF;
    return reg;
}

uint8_t cpu8085::INR_A() {
    return a = inrDcrReg(*this, a);
}

uint8_t cpu8085::INR_B() {
    return b = inrDcrReg(*this, b);
}

uint8_t cpu8085::INR_C() {
    return c = inrDcrReg(*this, c);
}

uint8_t cpu8085::INR_D() {
    return d = inrDcrReg(*this, d);
}

uint8_t cpu8085::INR_E() {
    return e = inrDcrReg(*this, e);
}

uint8_t cpu8085::INR_H() {
    return h = inrDcrReg(*this, h);
}

uint8_t cpu8085::INR_L() {
    return l = inrDcrReg(*this, l);
}

uint8_t cpu8085::INR_M() {
    uint16_t addr = (h << 8) | l;
    uint8_t val = inrDcrReg(*this, read(addr));
    write(addr, val);
    return val;
}

uint8_t cpu8085::DCR_A() {
    return a = inrDcrReg(*this, a, true);
}

uint8_t cpu8085::DCR_B() {
    return b = inrDcrReg(*this, b, true);
}

uint8_t cpu8085::DCR_C() {
    return c = inrDcrReg(*this, c, true);
}

uint8_t cpu8085::DCR_D() {
    return d = inrDcrReg(*this, d, true);
}

uint8_t cpu8085::DCR_E() {
    return e = inrDcrReg(*this, e, true);
}

uint8_t cpu8085::DCR_H() {
    return h = inrDcrReg(*this, h, true);
}

uint8_t cpu8085::DCR_L() {
    return l = inrDcrReg(*this, l, true);
}

uint8_t cpu8085::DCR_M() {
    uint16_t addr = (h << 8) | l;
    uint8_t val = inrDcrReg(*this, read(addr), true);
    write(addr, val);
    return val;
}

uint16_t inxDcxRegPair(uint8_t &reg1, uint8_t &reg2, bool inx = true) {
    uint16_t regPairVal = (reg1 << 8) | reg2;
    regPairVal = regPairVal + (inx ? 1 : -1);

    reg1 = (regPairVal >> 8) & 0xFF;
    reg2 = regPairVal & 0xFF;
    return regPairVal;
}

uint16_t cpu8085::INX_B() {
    return inxDcxRegPair(b, c);
}

uint16_t cpu8085::DCX_B() {
    return inxDcxRegPair(b, c, false);
}

uint16_t cpu8085::INX_D() {
    return inxDcxRegPair(b, d);
}

uint16_t cpu8085::DCX_D() {
    return inxDcxRegPair(d, e, false);
}

uint16_t cpu8085::INX_H() {
    return inxDcxRegPair(h, l);
}

uint16_t cpu8085::DCX_H() {
    return inxDcxRegPair(h, l, false);
}

uint16_t cpu8085::INX_SP() {
    return stkp = stkp + 1;
}

uint16_t cpu8085::DCX_SP() {
    return stkp = stkp - 1;
}

uint8_t cpu8085::DAA() {
    uint8_t lowerNibble = a & 0x0F;
    if (lowerNibble > 9 || GetFlag(FLAGS8085::A)) {
        bool aux_flag_cond = (lowerNibble + 6) > 0x0F;
        uint16_t temp = a + 0x06;
        allSetFlags(temp, aux_flag_cond);
        a = temp & 0xFF;
    }

    uint8_t upperNibble = a & 0xF0;
    if (upperNibble > 9 || GetFlag(FLAGS8085::C)) {
        uint16_t temp = a + 0x60;
        allSetFlags(temp, false);
        a = temp & 0xFF;
    }

    return a;
}

uint8_t cpu8085::CMC() {
    SetFlag(FLAGS8085::C, !GetFlag(FLAGS8085::C));
    return 0;
}

uint8_t cpu8085::STC() {
    SetFlag(FLAGS8085::C, true);
    return 0;
}

uint8_t cpu8085::CMA() {
    a = ~a;
    return a;
}

uint8_t cpu8085::RLC() {
    bool msb = a & 0x80;
    a = (a << 1) | msb;
    SetFlag(FLAGS8085::C, msb);
    return a;
}

uint8_t cpu8085::RRC() {
    bool lsb = a & 0x01;
    a = (a >> 1) | (lsb << 7);
    SetFlag(FLAGS8085::C, lsb);
    return a;
}

uint8_t cpu8085::RAL() {
    bool msb = a & 0x80;
    a = (a << 1) | GetFlag(FLAGS8085::C);
    SetFlag(FLAGS8085::C, msb);
    return a;
}

uint8_t cpu8085::RAR() {
    bool lsb = a & 0x01;
    a = (a >> 1) | (GetFlag(FLAGS8085::C) << 7);
    SetFlag(FLAGS8085::C, lsb);
    return a;
}
