#include "cpu8085.h"
#include "Bus.h"

uint8_t logicalOperationFlagSetter(cpu8085 &cpu8085, uint8_t val, bool cFlag, bool aFlag) {
    cpu8085.a = val;
    cpu8085.SetFlag(cpu8085::Z, cpu8085.a == 0x00);
    cpu8085.SetFlag(cpu8085::S, cpu8085.a & 0x80);
    cpu8085.SetFlag(cpu8085::P, cpu8085.a % 2 == 0);
    cpu8085.SetFlag(cpu8085::C, cFlag);
    cpu8085.SetFlag(cpu8085::A, aFlag);
    return cpu8085.a;
}

uint8_t cpu8085::ANA_A(){
    return logicalOperationFlagSetter(*this, a & a, false, true);
}

uint8_t cpu8085::ANA_B(){
    return logicalOperationFlagSetter(*this, b & a, false, true);
}

uint8_t cpu8085::ANA_C(){
    return logicalOperationFlagSetter(*this, c & a, false, true);
}

uint8_t cpu8085::ANA_D(){
    return logicalOperationFlagSetter(*this, d & a, false, true);
}

uint8_t cpu8085::ANA_E(){
    return logicalOperationFlagSetter(*this, e & a, false, true);
}

uint8_t cpu8085::ANA_H(){
    return logicalOperationFlagSetter(*this, h & a, false, true);
}

uint8_t cpu8085::ANA_L(){
    return logicalOperationFlagSetter(*this, l & a, false, true);
}

uint8_t cpu8085::ANA_M(){
    uint16_t addr = (h << 8) | l;
    return logicalOperationFlagSetter(*this, read(addr) & a, false, true);
}

uint8_t cpu8085::ANI() {
    uint8_t val = read(pc++);
    return logicalOperationFlagSetter(*this, val & a, false, true);
}

uint8_t cpu8085::XRA_A(){
    return logicalOperationFlagSetter(*this, a ^ a, false, false);
}

uint8_t cpu8085::XRA_B(){
    return logicalOperationFlagSetter(*this, b ^ a, false, false);
}

uint8_t cpu8085::XRA_C(){
    return logicalOperationFlagSetter(*this, c ^ a, false, false);
}

uint8_t cpu8085::XRA_D() {
    return logicalOperationFlagSetter(*this, d & a, false, false);
}

uint8_t cpu8085::XRA_E() {
    return logicalOperationFlagSetter(*this, e & a, false, false);
}

uint8_t cpu8085::XRA_H() {
    return logicalOperationFlagSetter(*this, h & a, false, false);
}

uint8_t cpu8085::XRA_L() {
    return logicalOperationFlagSetter(*this, l & a, false, false);
}

uint8_t cpu8085::XRA_M() {
    uint16_t addr = (h << 8) | l;
    return logicalOperationFlagSetter(*this, read(addr) & a, false, false);
}

uint8_t cpu8085::XRI() {
    uint8_t val = read(pc++);
    return logicalOperationFlagSetter(*this, val ^ a, false, false);
}

uint8_t cpu8085::ORA_A(){
    return logicalOperationFlagSetter(*this, a | a, false, false);
}

uint8_t cpu8085::ORA_B(){
    return logicalOperationFlagSetter(*this, b | a, false, false);
}

uint8_t cpu8085::ORA_C(){
    return logicalOperationFlagSetter(*this, c | a, false, false);
}

uint8_t cpu8085::ORA_D(){
    return logicalOperationFlagSetter(*this, d | a, false, false);
}

uint8_t cpu8085::ORA_E(){
    return logicalOperationFlagSetter(*this, e | a, false, false);
}

uint8_t cpu8085::ORA_H(){
    return logicalOperationFlagSetter(*this, h | a, false, false);
}

uint8_t cpu8085::ORA_L(){
    return logicalOperationFlagSetter(*this, l | a, false, false);
}

uint8_t cpu8085::ORA_M(){
    uint16_t addr = (h << 8) | l;
    return logicalOperationFlagSetter(*this, read(addr) | a, false, false);
}

uint8_t cpu8085::ORI() {
    uint8_t val = read(pc++);
    return logicalOperationFlagSetter(*this, val | a, false, false);
}

void cmpFlagSetter(cpu8085 &cpu8085, uint8_t val) {
    uint8_t temp = cpu8085.a - val;
    bool aux_flag_cond = (cpu8085.a & 0x0F) - (val & 0x0F) > 0x0F;
    cpu8085.allSetFlags(temp, aux_flag_cond);
}

uint8_t cpu8085::CMP_A(){
    cmpFlagSetter(*this, a);
    return a;
}

uint8_t cpu8085::CMP_B(){
    cmpFlagSetter(*this, b);
    return b;
}

uint8_t cpu8085::CMP_C(){
    cmpFlagSetter(*this, c);
    return c;
}

uint8_t cpu8085::CMP_D(){
    cmpFlagSetter(*this, d);
    return d;
}

uint8_t cpu8085::CMP_E(){
    cmpFlagSetter(*this, e);
    return e;
}

uint8_t cpu8085::CMP_H(){
    cmpFlagSetter(*this, h);
    return h;
}

uint8_t cpu8085::CMP_L(){
    cmpFlagSetter(*this, l);
    return l;
}

uint8_t cpu8085::CMP_M(){
    uint16_t addr = (h << 8) | l;
    cmpFlagSetter(*this, read(addr));
    return read(addr);
}

uint8_t cpu8085::CPI() {
    uint8_t val = read(pc++);
    cmpFlagSetter(*this, val);
    return val;
}