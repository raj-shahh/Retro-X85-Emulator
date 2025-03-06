#include "cpu8085.h"
#include "Bus.h"

// Constructor
cpu8085::cpu8085() 
{
  
	lookup.push_back(INSTRUCTION("NOP",     &cpu8085::NOP,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("LXI B",   &cpu8085::LXI_B,  &cpu8085::IMD_16, 3 ));
	lookup.push_back(INSTRUCTION("STAX B",  &cpu8085::STAX_B, &cpu8085::XXX,    2 ));
	lookup.push_back(INSTRUCTION("INX B",   &cpu8085::INX_B,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("INR B",   &cpu8085::INR_B,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("DCR B",   &cpu8085::DCR_B,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("MVI B",   &cpu8085::MVI_B,  &cpu8085::IMD_8,  2 ));
	lookup.push_back(INSTRUCTION("RLC",     &cpu8085::RLC,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("NOP",     &cpu8085::NOP,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("DAD B",   &cpu8085::DAD_B,  &cpu8085::XXX,    3 ));
	lookup.push_back(INSTRUCTION("LDAX B",  &cpu8085::LDAX_B, &cpu8085::XXX,    2 ));
	lookup.push_back(INSTRUCTION("DCX B",   &cpu8085::DCX_B,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("INR C",   &cpu8085::INR_C,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("DCR C",   &cpu8085::DCR_C,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("MVI C",   &cpu8085::MVI_C,  &cpu8085::IMD_8,  2 ));
	lookup.push_back(INSTRUCTION("RRC",     &cpu8085::RRC,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("NOP",     &cpu8085::NOP,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("LXI D",   &cpu8085::LXI_D,  &cpu8085::IMD_16, 3 ));
	lookup.push_back(INSTRUCTION("STAX D",  &cpu8085::STAX_D, &cpu8085::XXX,    2 ));
	lookup.push_back(INSTRUCTION("INX D",   &cpu8085::INX_D,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("INR D",   &cpu8085::INR_D,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("DCR D",   &cpu8085::DCR_D,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("MVI D",   &cpu8085::MVI_D,  &cpu8085::IMD_8,  2 ));
	lookup.push_back(INSTRUCTION("RAL",     &cpu8085::RAL,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("NOP",     &cpu8085::NOP,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("DAD D",   &cpu8085::DAD_D,  &cpu8085::XXX,    3 ));
	lookup.push_back(INSTRUCTION("LDAX D",  &cpu8085::LDAX_D, &cpu8085::XXX,    2 ));
	lookup.push_back(INSTRUCTION("DCX D",   &cpu8085::DCX_D,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("INR E",   &cpu8085::INR_E,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("DCR E",   &cpu8085::DCR_E,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("MVI E",   &cpu8085::MVI_E,  &cpu8085::IMD_8,  2 ));
	lookup.push_back(INSTRUCTION("RAR",     &cpu8085::RAR,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("RIM",     &cpu8085::RIM,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("LXI H",   &cpu8085::LXI_H,  &cpu8085::IMD_16, 3 ));
	lookup.push_back(INSTRUCTION("SHLD",    &cpu8085::SHLD,   &cpu8085::IMA,    4 ));
	lookup.push_back(INSTRUCTION("INX H",   &cpu8085::INX_H,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("INR H",   &cpu8085::INR_H,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("DCR H",   &cpu8085::DCR_H,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("MVI H",   &cpu8085::MVI_H,  &cpu8085::IMD_8,  2 ));
	lookup.push_back(INSTRUCTION("DAA",     &cpu8085::DAA,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("NOP",     &cpu8085::NOP,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("DAD H",   &cpu8085::DAD_H,  &cpu8085::XXX,    3 ));
	lookup.push_back(INSTRUCTION("LHLD",    &cpu8085::LHLD,   &cpu8085::IMA,    4 ));
	lookup.push_back(INSTRUCTION("DCX H",   &cpu8085::DCX_H,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("INR L",   &cpu8085::INR_L,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("DCR L",   &cpu8085::DCR_L,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("MVI L",   &cpu8085::MVI_L,  &cpu8085::IMD_8,  2 ));
	lookup.push_back(INSTRUCTION("CMA",     &cpu8085::CMA,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("SIM",     &cpu8085::SIM,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("LXI SP",  &cpu8085::LXI_SP, &cpu8085::IMD_16, 3 ));
	lookup.push_back(INSTRUCTION("STA",     &cpu8085::STA,    &cpu8085::IMA,    3 ));
	lookup.push_back(INSTRUCTION("INX SP",  &cpu8085::INX_SP, &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("INR M",   &cpu8085::INR_M,  &cpu8085::XXX,    3 ));
	lookup.push_back(INSTRUCTION("DCR M",   &cpu8085::DCR_M,  &cpu8085::XXX,    3 ));
	lookup.push_back(INSTRUCTION("MVI M",   &cpu8085::MVI_M,  &cpu8085::IMD_8,  3 ));
	lookup.push_back(INSTRUCTION("STC",     &cpu8085::STC,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("NOP",     &cpu8085::NOP,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("DAD SP",  &cpu8085::DAD_SP, &cpu8085::XXX,    3 ));
	lookup.push_back(INSTRUCTION("LDA",     &cpu8085::LDA,    &cpu8085::IMA,    3 ));
	lookup.push_back(INSTRUCTION("DCX SP",  &cpu8085::DCX_SP, &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("INR A",   &cpu8085::INR_A,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("DCR A",   &cpu8085::DCR_A,  &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("MVI A",   &cpu8085::MVI_A,  &cpu8085::IMD_8,  2 ));
	lookup.push_back(INSTRUCTION("CMC",     &cpu8085::CMC,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("MOV B,B", &cpu8085::MOV_B_B, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV B,C", &cpu8085::MOV_B_C, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV B,D", &cpu8085::MOV_B_D, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV B,E", &cpu8085::MOV_B_E, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV B,H", &cpu8085::MOV_B_H, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV B,L", &cpu8085::MOV_B_L, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV B,M", &cpu8085::MOV_B_M, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV B,A", &cpu8085::MOV_B_A, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV C,B", &cpu8085::MOV_C_B, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV C,C", &cpu8085::MOV_C_C, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV C,D", &cpu8085::MOV_C_D, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV C,E", &cpu8085::MOV_C_E, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV C,H", &cpu8085::MOV_C_H, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV C,L", &cpu8085::MOV_C_L, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV C,M", &cpu8085::MOV_C_M, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV C,A", &cpu8085::MOV_C_A, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV D,B", &cpu8085::MOV_D_B, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV D,C", &cpu8085::MOV_D_C, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV D,D", &cpu8085::MOV_D_D, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV D,E", &cpu8085::MOV_D_E, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV D,H", &cpu8085::MOV_D_H, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV D,L", &cpu8085::MOV_D_L, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV D,M", &cpu8085::MOV_D_M, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV D,A", &cpu8085::MOV_D_A, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV E,B", &cpu8085::MOV_E_B, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV E,C", &cpu8085::MOV_E_C, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV E,D", &cpu8085::MOV_E_D, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV E,E", &cpu8085::MOV_E_E, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV E,H", &cpu8085::MOV_E_H, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV E,L", &cpu8085::MOV_E_L, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV E,M", &cpu8085::MOV_E_M, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV E,A", &cpu8085::MOV_E_A, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV H,B", &cpu8085::MOV_H_B, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV H,C", &cpu8085::MOV_H_C, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV H,D", &cpu8085::MOV_H_D, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV H,E", &cpu8085::MOV_H_E, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV H,H", &cpu8085::MOV_H_H, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV H,L", &cpu8085::MOV_H_L, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV H,M", &cpu8085::MOV_H_M, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV H,A", &cpu8085::MOV_H_A, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV L,B", &cpu8085::MOV_L_B, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV L,C", &cpu8085::MOV_L_C, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV L,D", &cpu8085::MOV_L_D, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV L,E", &cpu8085::MOV_L_E, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV L,H", &cpu8085::MOV_L_H, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV L,L", &cpu8085::MOV_L_L, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV L,M", &cpu8085::MOV_L_M, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV L,A", &cpu8085::MOV_L_A, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV M,B", &cpu8085::MOV_M_B, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV M,C", &cpu8085::MOV_M_C, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV M,D", &cpu8085::MOV_M_D, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV M,E", &cpu8085::MOV_M_E, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV M,H", &cpu8085::MOV_M_H, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV M,L", &cpu8085::MOV_M_L, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("HLT",     &cpu8085::HLT,    &cpu8085::XXX,    1 ));
	lookup.push_back(INSTRUCTION("MOV M,A", &cpu8085::MOV_M_A, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV A,B", &cpu8085::MOV_A_B, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV A,C", &cpu8085::MOV_A_C, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV A,D", &cpu8085::MOV_A_D, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV A,E", &cpu8085::MOV_A_E, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV A,H", &cpu8085::MOV_A_H, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV A,L", &cpu8085::MOV_A_L, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV A,M", &cpu8085::MOV_A_M, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("MOV A,A", &cpu8085::MOV_A_A, &cpu8085::XXX,   1 ));
	lookup.push_back(INSTRUCTION("ADD B", &cpu8085::ADD_B, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADD C", &cpu8085::ADD_C, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADD D", &cpu8085::ADD_D, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADD E", &cpu8085::ADD_E, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADD H", &cpu8085::ADD_H, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADD L", &cpu8085::ADD_L, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADD M", &cpu8085::ADD_M, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADD A", &cpu8085::ADD_A, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADC B", &cpu8085::ADC_B, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADC C", &cpu8085::ADC_C, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADC D", &cpu8085::ADC_D, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADC E", &cpu8085::ADC_E, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADC H", &cpu8085::ADC_H, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADC L", &cpu8085::ADC_L, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADC M", &cpu8085::ADC_M, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADC A", &cpu8085::ADC_A, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SUB B", &cpu8085::SUB_B, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SUB C", &cpu8085::SUB_C, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SUB D", &cpu8085::SUB_D, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SUB E", &cpu8085::SUB_E, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SUB H", &cpu8085::SUB_H, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SUB L", &cpu8085::SUB_L, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SUB M", &cpu8085::SUB_M, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SUB A", &cpu8085::SUB_A, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SBB B", &cpu8085::SBB_B, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SBB C", &cpu8085::SBB_C, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SBB D", &cpu8085::SBB_D, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SBB E", &cpu8085::SBB_E, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SBB H", &cpu8085::SBB_H, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SBB L", &cpu8085::SBB_L, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SBB M", &cpu8085::SBB_M, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SBB A", &cpu8085::SBB_A, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ANA B", &cpu8085::ANA_B, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ANA C", &cpu8085::ANA_C, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ANA D", &cpu8085::ANA_D, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ANA E", &cpu8085::ANA_E, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ANA H", &cpu8085::ANA_H, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ANA L", &cpu8085::ANA_L, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ANA M", &cpu8085::ANA_M, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ANA A", &cpu8085::ANA_A, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("XRA B", &cpu8085::XRA_B, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("XRA C", &cpu8085::XRA_C, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("XRA D", &cpu8085::XRA_D, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("XRA E", &cpu8085::XRA_E, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("XRA H", &cpu8085::XRA_H, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("XRA L", &cpu8085::XRA_L, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("XRA M", &cpu8085::XRA_M, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("XRA A", &cpu8085::XRA_A, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ORA B", &cpu8085::ORA_B, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ORA C", &cpu8085::ORA_C, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ORA D", &cpu8085::ORA_D, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ORA E", &cpu8085::ORA_E, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ORA H", &cpu8085::ORA_H, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ORA L", &cpu8085::ORA_L, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ORA M", &cpu8085::ORA_M, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ORA A", &cpu8085::ORA_A, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("CMP B", &cpu8085::CMP_B, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("CMP C", &cpu8085::CMP_C, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("CMP D", &cpu8085::CMP_D, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("CMP E", &cpu8085::CMP_E, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("CMP H", &cpu8085::CMP_H, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("CMP L", &cpu8085::CMP_L, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("CMP M", &cpu8085::CMP_M, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("CMP A", &cpu8085::CMP_A, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("RNZ", &cpu8085::RNZ, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("POP B", &cpu8085::POP_B, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("JNZ", &cpu8085::JNZ, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("JMP", &cpu8085::JMP, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("CNZ", &cpu8085::CNZ, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("PUSH B", &cpu8085::PUSH_B, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ADI", &cpu8085::ADI, &cpu8085::IMD_8, 2 ));
	lookup.push_back(INSTRUCTION("RST 0", &cpu8085::RST_0, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("RZ", &cpu8085::RZ, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("RET", &cpu8085::RET, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("JZ", &cpu8085::JZ, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("NOP", &cpu8085::NOP, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("CZ", &cpu8085::CZ, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("CALL", &cpu8085::CALL, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("ACI", &cpu8085::ACI, &cpu8085::IMD_8, 2 ));
	lookup.push_back(INSTRUCTION("RST 1", &cpu8085::RST_1, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("RNC", &cpu8085::RNC, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("POP D", &cpu8085::POP_D, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("JNC", &cpu8085::JNC, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("OUT", &cpu8085::OUT, &cpu8085::IMD_8, 2 ));
	lookup.push_back(INSTRUCTION("CNC", &cpu8085::CNC, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("PUSH D", &cpu8085::PUSH_D, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SUI", &cpu8085::SUI, &cpu8085::IMD_8, 2 ));
	lookup.push_back(INSTRUCTION("RST 2", &cpu8085::RST_2, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("RC", &cpu8085::RC, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("NOP", &cpu8085::NOP, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("JC", &cpu8085::JC, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("IN", &cpu8085::IN, &cpu8085::IMD_8, 2 ));
	lookup.push_back(INSTRUCTION("CC", &cpu8085::CC, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("NOP", &cpu8085::NOP, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SBI", &cpu8085::SBI, &cpu8085::IMD_8, 2 ));
	lookup.push_back(INSTRUCTION("RST 3", &cpu8085::RST_3, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("RPO", &cpu8085::RPO, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("POP H", &cpu8085::POP_H, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("JPO", &cpu8085::JPO, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("XTHL", &cpu8085::XTHL, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("CPO", &cpu8085::CPO, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("PUSH H", &cpu8085::PUSH_H, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ANI", &cpu8085::ANI, &cpu8085::IMD_8, 2 ));
	lookup.push_back(INSTRUCTION("RST 4", &cpu8085::RST_4, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("RPE", &cpu8085::RPE, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("PCHL", &cpu8085::PCHL, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("JPE", &cpu8085::JPE, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("XCHG", &cpu8085::XCHG, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("CPE", &cpu8085::CPE, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("NOP", &cpu8085::NOP, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("XRI", &cpu8085::XRI, &cpu8085::IMD_8, 2 ));
	lookup.push_back(INSTRUCTION("RST 5", &cpu8085::RST_5, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("RP", &cpu8085::RP, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("POP PSW", &cpu8085::POP_PSW, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("JP", &cpu8085::JP, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("DI", &cpu8085::DI, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("CP", &cpu8085::CP, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("PUSH PSW", &cpu8085::PUSH_PSW, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("ORI", &cpu8085::ORI, &cpu8085::IMD_8, 2 ));
	lookup.push_back(INSTRUCTION("RST 6", &cpu8085::RST_6, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("RM", &cpu8085::RM, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("SPHL", &cpu8085::SPHL, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("JM", &cpu8085::JM, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("EI", &cpu8085::EI, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("CM", &cpu8085::CM, &cpu8085::IMA, 3 ));
	lookup.push_back(INSTRUCTION("NOP", &cpu8085::NOP, &cpu8085::XXX, 1 ));
	lookup.push_back(INSTRUCTION("CPI", &cpu8085::CPI, &cpu8085::IMD_8, 2 ));
	lookup.push_back(INSTRUCTION("RST 7", &cpu8085::RST_7, &cpu8085::XXX, 1 ));
	
}

// Destructor - has nothing to do
cpu8085::~cpu8085()
{
	;
}

