#include "cpu8085.h"
#include "Bus.h"

//reg1 is always accumulator
uint8_t addSubRegisters(cpu8085 &cpu8085, uint8_t reg2, bool carry = false, bool subtract = false){
    uint16_t tempComputation = cpu8085.a + (-1)^subtract * (reg2 + carry);
    bool auxFlagCond = (cpu8085.a & 0x0F) + (-1)^subtract *(reg2 & 0x0F) + (-1)^subtract *(carry & 0x0F) > 0x0F;
    cpu8085.allSetFlags(tempComputation, auxFlagCond);
    cpu8085.a = tempComputation & 0xFF;
    return cpu8085.a;
}

uint8_t cpu8085::ADD_A(){
    return addSubRegisters(*this, a);
}

uint8_t cpu8085::ADD_B(){
    return addSubRegisters(*this, b);
}

uint8_t cpu8085::ADD_C(){
    return addSubRegisters(*this, c);
}

uint8_t cpu8085::ADD_D(){
    return addSubRegisters(*this, d);
}

uint8_t cpu8085::ADD_E(){
    return addSubRegisters(*this, e);
}

uint8_t cpu8085::ADD_H(){
    return addSubRegisters(*this, h);
}

uint8_t cpu8085::ADD_L(){
    return addSubRegisters(*this, l);
}

uint8_t cpu8085::ADD_M(){
    uint16_t addr = (h << 8) | l;
    return addSubRegisters(*this, read(addr));
}

uint8_t cpu8085::ADI() {
    uint8_t val = read(pc++);
    return addSubRegisters(*this, val);
}

uint8_t cpu8085::ADC_A(){
    return addSubRegisters(*this, a, GetFlag(FLAGS8085::C));
}

uint8_t cpu8085::ADC_B(){
    return addSubRegisters(*this, b, GetFlag(FLAGS8085::C));
}

uint8_t cpu8085::ADC_C(){
    return addSubRegisters(*this, c, GetFlag(FLAGS8085::C));
}

uint8_t cpu8085::ADC_D(){
    return addSubRegisters(*this, d, GetFlag(FLAGS8085::C));
}

uint8_t cpu8085::ADC_E(){
    return addSubRegisters(*this, e, GetFlag(FLAGS8085::C));
}

uint8_t cpu8085::ADC_H(){
    return addSubRegisters(*this, h, GetFlag(FLAGS8085::C));
}

uint8_t cpu8085::ADC_L(){
    return addSubRegisters(*this, l, GetFlag(FLAGS8085::C));
}

uint8_t cpu8085::ADC_M(){
    uint16_t addr = (h << 8) | l;
    return addSubRegisters(*this, read(addr), GetFlag(FLAGS8085::C));
}

uint8_t cpu8085::ACI() {
    uint8_t val = read(pc++);
    return addSubRegisters(*this, val, GetFlag(FLAGS8085::C));
}

uint8_t cpu8085::SUB_A() {
    return addSubRegisters(*this, a, false, true);
}

uint8_t cpu8085::SUB_B() {
    return addSubRegisters(*this, b, false, true);
}

uint8_t cpu8085::SUB_C() {
    return addSubRegisters(*this, c, false, true);
}

uint8_t cpu8085::SUB_D() {
    return addSubRegisters(*this, d, false, true);
}

uint8_t cpu8085::SUB_E() {
    return addSubRegisters(*this, e, false, true);
}

uint8_t cpu8085::SUB_H() {
    return addSubRegisters(*this, h, false, true);
}

uint8_t cpu8085::SUB_L() {
    return addSubRegisters(*this, l, false, true);
}

uint8_t cpu8085::SUB_M() {
    uint16_t addr = (h << 8) | l;
    return addSubRegisters(*this, read(addr), false, true);
}

uint8_t cpu8085::SUI() {
    uint8_t val = read(pc++);
    return addSubRegisters(*this, val, false, true);
}

uint8_t cpu8085::SBB_A() {
    return addSubRegisters(*this, a, GetFlag(FLAGS8085::C), true);
}

uint8_t cpu8085::SBB_B() {
    return addSubRegisters(*this, b, GetFlag(FLAGS8085::C), true);
}

uint8_t cpu8085::SBB_C() {
    return addSubRegisters(*this, c, GetFlag(FLAGS8085::C), true);
}

uint8_t cpu8085::SBB_D() {
    return addSubRegisters(*this, d, GetFlag(FLAGS8085::C), true);
}

uint8_t cpu8085::SBB_E() {
    return addSubRegisters(*this, e, GetFlag(FLAGS8085::C), true);
}

uint8_t cpu8085::SBB_H() {
    return addSubRegisters(*this, h, GetFlag(FLAGS8085::C), true);
}

uint8_t cpu8085::SBB_L() {
    return addSubRegisters(*this, l, GetFlag(FLAGS8085::C), true);
}

uint8_t cpu8085::SBB_M() {
    uint16_t addr = (h << 8) | l;
    return addSubRegisters(*this, read(addr), GetFlag(FLAGS8085::C), true);
}

uint8_t cpu8085::SBI() {
    uint8_t val = read(pc++);
    return addSubRegisters(*this, val, GetFlag(FLAGS8085::C), true);
}

uint16_t DAD(cpu8085& cpu8085, uint16_t regPair){
    uint16_t hl = (cpu8085.h << 8) | cpu8085.l;
    uint32_t tempComputation = hl + regPair;
    cpu8085.SetFlag(cpu8085.FLAGS8085::C, tempComputation > 0xFFFF);
    cpu8085.h = (tempComputation >> 8) & 0xFF;
    cpu8085.l = tempComputation & 0xFF;
    return (cpu8085.h << 8) | cpu8085.l;
}

uint16_t cpu8085::DAD_B(){
    uint16_t bc = (b << 8) | c;
    return DAD(*this, bc);
}

uint16_t cpu8085::DAD_D(){
    uint16_t de = (d << 8) | e;
    return DAD(*this, de);
}

uint16_t cpu8085::DAD_H(){
    uint16_t hl = (h << 8) | l;
    return DAD(*this, hl);
}

uint16_t cpu8085::DAD_SP(){
    uint16_t hl = (h << 8) | l;
    return DAD(*this, stkp);
}



