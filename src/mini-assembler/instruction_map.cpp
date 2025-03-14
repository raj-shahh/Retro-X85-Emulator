#include "assembler.h"

Assembler::Assembler() {
    instructionToOpcode["NOP"] = 0x00;
    instructionToOpcode["LXI B"] = 0x01;
    instructionToOpcode["STAX B"] = 0x02;
    instructionToOpcode["INX B"] = 0x03;
    instructionToOpcode["INR B"] = 0x04;
    instructionToOpcode["DCR B"] = 0x05;
    instructionToOpcode["MVI B"] = 0x06;
    instructionToOpcode["RLC"] = 0x07;
    instructionToOpcode["DAD B"] = 0x09;
    instructionToOpcode["LDAX B"] = 0x0A;
    instructionToOpcode["DCX B"] = 0x0B;
    instructionToOpcode["INR C"] = 0x0C;
    instructionToOpcode["DCR C"] = 0x0D;
    instructionToOpcode["MVI C"] = 0x0E;
    instructionToOpcode["RRC"] = 0x0F;
    instructionToOpcode["LXI D"] = 0x11;
    instructionToOpcode["STAX D"] = 0x12;
    instructionToOpcode["INX D"] = 0x13;
    instructionToOpcode["INR D"] = 0x14;
    instructionToOpcode["DCR D"] = 0x15;
    instructionToOpcode["MVI D"] = 0x16;
    instructionToOpcode["RAL"] = 0x17;
    instructionToOpcode["DAD D"] = 0x19;
    instructionToOpcode["LDAX D"] = 0x1A;
    instructionToOpcode["DCX D"] = 0x1B;
    instructionToOpcode["INR E"] = 0x1C;
    instructionToOpcode["DCR E"] = 0x1D;
    instructionToOpcode["MVI E"] = 0x1E;
    instructionToOpcode["RAR"] = 0x1F;
    instructionToOpcode["RIM"] = 0x20;
    instructionToOpcode["LXI H"] = 0x21;
    instructionToOpcode["SHLD"] = 0x22;
    instructionToOpcode["INX H"] = 0x23;
    instructionToOpcode["INR H"] = 0x24;
    instructionToOpcode["DCR H"] = 0x25;
    instructionToOpcode["MVI H"] = 0x26;
    instructionToOpcode["DAA"] = 0x27;
    instructionToOpcode["DAD H"] = 0x29;
    instructionToOpcode["LHLD"] = 0x2A;
    instructionToOpcode["DCX H"] = 0x2B;
    instructionToOpcode["INR L"] = 0x2C;
    instructionToOpcode["DCR L"] = 0x2D;
    instructionToOpcode["MVI L"] = 0x2E;
    instructionToOpcode["CMA"] = 0x2F;
    instructionToOpcode["SIM"] = 0x30;
    instructionToOpcode["LXI SP"] = 0x31;
    instructionToOpcode["STA"] = 0x32;
    instructionToOpcode["INX SP"] = 0x33;
    instructionToOpcode["INR M"] = 0x34;
    instructionToOpcode["DCR M"] = 0x35;
    instructionToOpcode["MVI M"] = 0x36;
    instructionToOpcode["STC"] = 0x37;
    instructionToOpcode["DAD SP"] = 0x39;
    instructionToOpcode["LDA"] = 0x3A;
    instructionToOpcode["DCX SP"] = 0x3B;
    instructionToOpcode["INR A"] = 0x3C;
    instructionToOpcode["DCR A"] = 0x3D;
    instructionToOpcode["MVI A"] = 0x3E;
    instructionToOpcode["CMC"] = 0x3F;
    instructionToOpcode["MOV B,B"] = 0x40;
    instructionToOpcode["MOV B,C"] = 0x41;
    instructionToOpcode["MOV B,D"] = 0x42;
    instructionToOpcode["MOV B,E"] = 0x43;
    instructionToOpcode["MOV B,H"] = 0x44;
    instructionToOpcode["MOV B,L"] = 0x45;
    instructionToOpcode["MOV B,M"] = 0x46;
    instructionToOpcode["MOV B,A"] = 0x47;
    instructionToOpcode["MOV C,B"] = 0x48;
    instructionToOpcode["MOV C,C"] = 0x49;
    instructionToOpcode["MOV C,D"] = 0x4A;
    instructionToOpcode["MOV C,E"] = 0x4B;
    instructionToOpcode["MOV C,H"] = 0x4C;
    instructionToOpcode["MOV C,L"] = 0x4D;
    instructionToOpcode["MOV C,M"] = 0x4E;
    instructionToOpcode["MOV C,A"] = 0x4F;
    instructionToOpcode["MOV D,B"] = 0x50;
    instructionToOpcode["MOV D,C"] = 0x51;
    instructionToOpcode["MOV D,D"] = 0x52;
    instructionToOpcode["MOV D,E"] = 0x53;
    instructionToOpcode["MOV D,H"] = 0x54;
    instructionToOpcode["MOV D,L"] = 0x55;
    instructionToOpcode["MOV D,M"] = 0x56;
    instructionToOpcode["MOV D,A"] = 0x57;
    instructionToOpcode["MOV E,B"] = 0x58;
    instructionToOpcode["MOV E,C"] = 0x59;
    instructionToOpcode["MOV E,D"] = 0x5A;
    instructionToOpcode["MOV E,E"] = 0x5B;
    instructionToOpcode["MOV E,H"] = 0x5C;
    instructionToOpcode["MOV E,L"] = 0x5D;
    instructionToOpcode["MOV E,M"] = 0x5E;
    instructionToOpcode["MOV E,A"] = 0x5F;
    instructionToOpcode["MOV H,B"] = 0x60;
    instructionToOpcode["MOV H,C"] = 0x61;
    instructionToOpcode["MOV H,D"] = 0x62;
    instructionToOpcode["MOV H,E"] = 0x63;
    instructionToOpcode["MOV H,H"] = 0x64;
    instructionToOpcode["MOV H,L"] = 0x65;
    instructionToOpcode["MOV H,M"] = 0x66;
    instructionToOpcode["MOV H,A"] = 0x67;
    instructionToOpcode["MOV L,B"] = 0x68;
    instructionToOpcode["MOV L,C"] = 0x69;
    instructionToOpcode["MOV L,D"] = 0x6A;
    instructionToOpcode["MOV L,E"] = 0x6B;
    instructionToOpcode["MOV L,H"] = 0x6C;
    instructionToOpcode["MOV L,L"] = 0x6D;
    instructionToOpcode["MOV L,M"] = 0x6E;
    instructionToOpcode["MOV L,A"] = 0x6F;
    instructionToOpcode["MOV M,B"] = 0x70;
    instructionToOpcode["MOV M,C"] = 0x71;
    instructionToOpcode["MOV M,D"] = 0x72;
    instructionToOpcode["MOV M,E"] = 0x73;
    instructionToOpcode["MOV M,H"] = 0x74;
    instructionToOpcode["MOV M,L"] = 0x75;
    instructionToOpcode["HLT"] = 0x76;
    instructionToOpcode["MOV M,A"] = 0x77;
    instructionToOpcode["MOV A,B"] = 0x78;
    instructionToOpcode["MOV A,C"] = 0x79;
    instructionToOpcode["MOV A,D"] = 0x7A;
    instructionToOpcode["MOV A,E"] = 0x7B;
    instructionToOpcode["MOV A,H"] = 0x7C;
    instructionToOpcode["MOV A,L"] = 0x7D;
    instructionToOpcode["MOV A,M"] = 0x7E;
    instructionToOpcode["MOV A,A"] = 0x7F;
    instructionToOpcode["ADD B"] = 0x80;
    instructionToOpcode["ADD C"] = 0x81;
    instructionToOpcode["ADD D"] = 0x82;
    instructionToOpcode["ADD E"] = 0x83;
    instructionToOpcode["ADD H"] = 0x84;
    instructionToOpcode["ADD L"] = 0x85;
    instructionToOpcode["ADD M"] = 0x86;
    instructionToOpcode["ADD A"] = 0x87;
    instructionToOpcode["ADC B"] = 0x88;
    instructionToOpcode["ADC C"] = 0x89;
    instructionToOpcode["ADC D"] = 0x8A;
    instructionToOpcode["ADC E"] = 0x8B;
    instructionToOpcode["ADC H"] = 0x8C;
    instructionToOpcode["ADC L"] = 0x8D;
    instructionToOpcode["ADC M"] = 0x8E;
    instructionToOpcode["ADC A"] = 0x8F;
    instructionToOpcode["SUB B"] = 0x90;
    instructionToOpcode["SUB C"] = 0x91;
    instructionToOpcode["SUB D"] = 0x92;
    instructionToOpcode["SUB E"] = 0x93;
    instructionToOpcode["SUB H"] = 0x94;
    instructionToOpcode["SUB L"] = 0x95;
    instructionToOpcode["SUB M"] = 0x96;
    instructionToOpcode["SUB A"] = 0x97;
    instructionToOpcode["SBB B"] = 0x98;
    instructionToOpcode["SBB C"] = 0x99;
    instructionToOpcode["SBB D"] = 0x9A;
    instructionToOpcode["SBB E"] = 0x9B;
    instructionToOpcode["SBB H"] = 0x9C;
    instructionToOpcode["SBB L"] = 0x9D;
    instructionToOpcode["SBB M"] = 0x9E;
    instructionToOpcode["SBB A"] = 0x9F;
    instructionToOpcode["ANA B"] = 0xA0;
    instructionToOpcode["ANA C"] = 0xA1;
    instructionToOpcode["ANA D"] = 0xA2;
    instructionToOpcode["ANA E"] = 0xA3;
    instructionToOpcode["ANA H"] = 0xA4;
    instructionToOpcode["ANA L"] = 0xA5;
    instructionToOpcode["ANA M"] = 0xA6;
    instructionToOpcode["ANA A"] = 0xA7;
    instructionToOpcode["XRA B"] = 0xA8;
    instructionToOpcode["XRA C"] = 0xA9;
    instructionToOpcode["XRA D"] = 0xAA;
    instructionToOpcode["XRA E"] = 0xAB;
    instructionToOpcode["XRA H"] = 0xAC;
    instructionToOpcode["XRA L"] = 0xAD;
    instructionToOpcode["XRA M"] = 0xAE;
    instructionToOpcode["XRA A"] = 0xAF;
    instructionToOpcode["ORA B"] = 0xB0;
    instructionToOpcode["ORA C"] = 0xB1;
    instructionToOpcode["ORA D"] = 0xB2;
    instructionToOpcode["ORA E"] = 0xB3;
    instructionToOpcode["ORA H"] = 0xB4;
    instructionToOpcode["ORA L"] = 0xB5;
    instructionToOpcode["ORA M"] = 0xB6;
    instructionToOpcode["ORA A"] = 0xB7;
    instructionToOpcode["CMP B"] = 0xB8;
    instructionToOpcode["CMP C"] = 0xB9;
    instructionToOpcode["CMP D"] = 0xBA;
    instructionToOpcode["CMP E"] = 0xBB;
    instructionToOpcode["CMP H"] = 0xBC;
    instructionToOpcode["CMP L"] = 0xBD;
    instructionToOpcode["CMP M"] = 0xBE;
    instructionToOpcode["CMP A"] = 0xBF;
    instructionToOpcode["RNZ"] = 0xC0;
    instructionToOpcode["POP B"] = 0xC1;
    instructionToOpcode["JNZ"] = 0xC2;
    instructionToOpcode["JMP"] = 0xC3;
    instructionToOpcode["CNZ"] = 0xC4;
    instructionToOpcode["PUSH B"] = 0xC5;
    instructionToOpcode["ADI"] = 0xC6;
    instructionToOpcode["RST 0"] = 0xC7;
    instructionToOpcode["RZ"] = 0xC8;
    instructionToOpcode["RET"] = 0xC9;
    instructionToOpcode["JZ"] = 0xCA;
    instructionToOpcode["CZ"] = 0xCC;
    instructionToOpcode["CALL"] = 0xCD;
    instructionToOpcode["ACI"] = 0xCE;
    instructionToOpcode["RST 1"] = 0xCF;
    instructionToOpcode["RNC"] = 0xD0;
    instructionToOpcode["POP D"] = 0xD1;
    instructionToOpcode["JNC"] = 0xD2;
    instructionToOpcode["OUT"] = 0xD3;
    instructionToOpcode["CNC"] = 0xD4;
    instructionToOpcode["PUSH D"] = 0xD5;
    instructionToOpcode["SUI"] = 0xD6;
    instructionToOpcode["RST 2"] = 0xD7;
    instructionToOpcode["RC"] = 0xD8;
    instructionToOpcode["JC"] = 0xDA;
    instructionToOpcode["IN"] = 0xDB;
    instructionToOpcode["CC"] = 0xDC;
    instructionToOpcode["SBI"] = 0xDE;
    instructionToOpcode["RST 3"] = 0xDF;
    instructionToOpcode["RPO"] = 0xE0;
    instructionToOpcode["POP H"] = 0xE1;
    instructionToOpcode["JPO"] = 0xE2;
    instructionToOpcode["XTHL"] = 0xE3;
    instructionToOpcode["CPO"] = 0xE4;
    instructionToOpcode["PUSH H"] = 0xE5;
    instructionToOpcode["ANI"] = 0xE6;
    instructionToOpcode["RST 4"] = 0xE7;
    instructionToOpcode["RPE"] = 0xE8;
    instructionToOpcode["PCHL"] = 0xE9;
    instructionToOpcode["JPE"] = 0xEA;
    instructionToOpcode["XCHG"] = 0xEB;
    instructionToOpcode["CPE"] = 0xEC;
    instructionToOpcode["XRI"] = 0xEE;
    instructionToOpcode["RST 5"] = 0xEF;
    instructionToOpcode["RP"] = 0xF0;
    instructionToOpcode["POP PSW"] = 0xF1;
    instructionToOpcode["JP"] = 0xF2;
    instructionToOpcode["DI"] = 0xF3;
    instructionToOpcode["CP"] = 0xF4;
    instructionToOpcode["PUSH PSW"] = 0xF5;
    instructionToOpcode["ORI"] = 0xF6;
    instructionToOpcode["RST 6"] = 0xF7;
    instructionToOpcode["RM"] = 0xF8;
    instructionToOpcode["SPHL"] = 0xF9;
    instructionToOpcode["JM"] = 0xFA;
    instructionToOpcode["EI"] = 0xFB;
    instructionToOpcode["CM"] = 0xFC;
    instructionToOpcode["CPI"] = 0xFE;
    instructionToOpcode["RST 7"] = 0xFF;
}