
#include "cpu8085.h"
#include "Bus.h"

// Constructor
cpu8085::cpu8085()
{


	// The table is one big initialiser list of initialiser lists...
	using a = cpu8085;
	lookup =
	{
	    { "NOP",     &cpu8085::NOP,    &cpu8085::XXX,    1 },
	    { "LXI B",   &cpu8085::LXI_B,  &cpu8085::IMD_16, 3 },
	    { "STAX B",  &cpu8085::STAX_B, &cpu8085::XXX,    2 },
	    { "INX B",   &cpu8085::INX_B,  &cpu8085::XXX,    1 },
	    { "INR B",   &cpu8085::INR_B,  &cpu8085::XXX,    1 },
	    { "DCR B",   &cpu8085::DCR_B,  &cpu8085::XXX,    1 },
	    { "MVI B",   &cpu8085::MVI_B,  &cpu8085::IMD_8,  2 },
	    { "RLC",     &cpu8085::RLC,    &cpu8085::XXX,    1 },
	    { "NOP",     &cpu8085::NOP,    &cpu8085::XXX,    1 },
	    { "DAD B",   &cpu8085::DAD_B,  &cpu8085::XXX,    3 },
	    { "LDAX B",  &cpu8085::LDAX_B, &cpu8085::XXX,    2 },
	    { "DCX B",   &cpu8085::DCX_B,  &cpu8085::XXX,    1 },
	    { "INR C",   &cpu8085::INR_C,  &cpu8085::XXX,    1 },
	    { "DCR C",   &cpu8085::DCR_C,  &cpu8085::XXX,    1 },
	    { "MVI C",   &cpu8085::MVI_C,  &cpu8085::IMD_8,  2 },
	    { "RRC",     &cpu8085::RRC,    &cpu8085::XXX,    1 },
	    { "NOP",     &cpu8085::NOP,    &cpu8085::XXX,    1 },
	    { "LXI D",   &cpu8085::LXI_D,  &cpu8085::IMD_16, 3 },
	    { "STAX D",  &cpu8085::STAX_D, &cpu8085::XXX,    2 },
	    { "INX D",   &cpu8085::INX_D,  &cpu8085::XXX,    1 },
	    { "INR D",   &cpu8085::INR_D,  &cpu8085::XXX,    1 },
	    { "DCR D",   &cpu8085::DCR_D,  &cpu8085::XXX,    1 },
	    { "MVI D",   &cpu8085::MVI_D,  &cpu8085::IMD_8,  2 },
	    { "RAL",     &cpu8085::RAL,    &cpu8085::XXX,    1 },
	    { "NOP",     &cpu8085::NOP,    &cpu8085::XXX,    1 },
	    { "DAD D",   &cpu8085::DAD_D,  &cpu8085::XXX,    3 },
	    { "LDAX D",  &cpu8085::LDAX_D, &cpu8085::XXX,    2 },
	    { "DCX D",   &cpu8085::DCX_D,  &cpu8085::XXX,    1 },
	    { "INR E",   &cpu8085::INR_E,  &cpu8085::XXX,    1 },
	    { "DCR E",   &cpu8085::DCR_E,  &cpu8085::XXX,    1 },
	    { "MVI E",   &cpu8085::MVI_E,  &cpu8085::IMD_8,  2 },
	    { "RAR",     &cpu8085::RAR,    &cpu8085::XXX,    1 },
	    { "RIM",     &cpu8085::RIM,    &cpu8085::XXX,    1 },
	    { "LXI H",   &cpu8085::LXI_H,  &cpu8085::IMD_16, 3 },
	    { "SHLD",    &cpu8085::SHLD,   &cpu8085::IMA,    4 },
	    { "INX H",   &cpu8085::INX_H,  &cpu8085::XXX,    1 },
	    { "INR H",   &cpu8085::INR_H,  &cpu8085::XXX,    1 },
	    { "DCR H",   &cpu8085::DCR_H,  &cpu8085::XXX,    1 },
	    { "MVI H",   &cpu8085::MVI_H,  &cpu8085::IMD_8,  2 },
	    { "DAA",     &cpu8085::DAA,    &cpu8085::XXX,    1 },
	    { "NOP",     &cpu8085::NOP,    &cpu8085::XXX,    1 },
	    { "DAD H",   &cpu8085::DAD_H,  &cpu8085::XXX,    3 },
	    { "LHLD",    &cpu8085::LHLD,   &cpu8085::IMA,    4 },
	    { "DCX H",   &cpu8085::DCX_H,  &cpu8085::XXX,    1 },
	    { "INR L",   &cpu8085::INR_L,  &cpu8085::XXX,    1 },
	    { "DCR L",   &cpu8085::DCR_L,  &cpu8085::XXX,    1 },
	    { "MVI L",   &cpu8085::MVI_L,  &cpu8085::IMD_8,  2 },
	    { "CMA",     &cpu8085::CMA,    &cpu8085::XXX,    1 },
	    { "SIM",     &cpu8085::SIM,    &cpu8085::XXX,    1 },
	    { "LXI SP",  &cpu8085::LXI_SP, &cpu8085::IMD_16, 3 },
	    { "STA",     &cpu8085::STA,    &cpu8085::IMA,    3 },
	    { "INX SP",  &cpu8085::INX_SP, &cpu8085::XXX,    1 },
	    { "INR M",   &cpu8085::INR_M,  &cpu8085::XXX,    3 },
	    { "DCR M",   &cpu8085::DCR_M,  &cpu8085::XXX,    3 },
	    { "MVI M",   &cpu8085::MVI_M,  &cpu8085::IMD_8,  3 },
	    { "STC",     &cpu8085::STC,    &cpu8085::XXX,    1 },
	    { "NOP",     &cpu8085::NOP,    &cpu8085::XXX,    1 },
	    { "DAD SP",  &cpu8085::DAD_SP, &cpu8085::XXX,    3 },
	    { "LDA",     &cpu8085::LDA,    &cpu8085::IMA,    3 },
	    { "DCX SP",  &cpu8085::DCX_SP, &cpu8085::XXX,    1 },
	    { "INR A",   &cpu8085::INR_A,  &cpu8085::XXX,    1 },
	    { "DCR A",   &cpu8085::DCR_A,  &cpu8085::XXX,    1 },
	    { "MVI A",   &cpu8085::MVI_A,  &cpu8085::IMD_8,  2 },
	    { "CMC",     &cpu8085::CMC,    &cpu8085::XXX,    1 },
	    { "MOV B,B", &cpu8085::MOV_B_B, &cpu8085::XXX,   1 },
	    { "MOV B,C", &cpu8085::MOV_B_C, &cpu8085::XXX,   1 },
	    { "MOV B,D", &cpu8085::MOV_B_D, &cpu8085::XXX,   1 },
	    { "MOV B,E", &cpu8085::MOV_B_E, &cpu8085::XXX,   1 },
	    { "MOV B,H", &cpu8085::MOV_B_H, &cpu8085::XXX,   1 },
	    { "MOV B,L", &cpu8085::MOV_B_L, &cpu8085::XXX,   1 },
	    { "MOV B,M", &cpu8085::MOV_B_M, &cpu8085::XXX,   1 },
	    { "MOV B,A", &cpu8085::MOV_B_A, &cpu8085::XXX,   1 },
	    { "MOV C,B", &cpu8085::MOV_C_B, &cpu8085::XXX,   1 },
	    { "MOV C,C", &cpu8085::MOV_C_C, &cpu8085::XXX,   1 },
	    { "MOV C,D", &cpu8085::MOV_C_D, &cpu8085::XXX,   1 },
	    { "MOV C,E", &cpu8085::MOV_C_E, &cpu8085::XXX,   1 },
	    { "MOV C,H", &cpu8085::MOV_C_H, &cpu8085::XXX,   1 },
	    { "MOV C,L", &cpu8085::MOV_C_L, &cpu8085::XXX,   1 },
	    { "MOV C,M", &cpu8085::MOV_C_M, &cpu8085::XXX,   1 },
	    { "MOV C,A", &cpu8085::MOV_C_A, &cpu8085::XXX,   1 },
	    { "MOV D,B", &cpu8085::MOV_D_B, &cpu8085::XXX,   1 },
	    { "MOV D,C", &cpu8085::MOV_D_C, &cpu8085::XXX,   1 },
	    { "MOV D,D", &cpu8085::MOV_D_D, &cpu8085::XXX,   1 },
	    { "MOV D,E", &cpu8085::MOV_D_E, &cpu8085::XXX,   1 },
	    { "MOV D,H", &cpu8085::MOV_D_H, &cpu8085::XXX,   1 },
	    { "MOV D,L", &cpu8085::MOV_D_L, &cpu8085::XXX,   1 },
	    { "MOV D,M", &cpu8085::MOV_D_M, &cpu8085::XXX,   1 },
	    { "MOV D,A", &cpu8085::MOV_D_A, &cpu8085::XXX,   1 },
	    { "MOV E,B", &cpu8085::MOV_E_B, &cpu8085::XXX,   1 },
	    { "MOV E,C", &cpu8085::MOV_E_C, &cpu8085::XXX,   1 },
	    { "MOV E,D", &cpu8085::MOV_E_D, &cpu8085::XXX,   1 },
	    { "MOV E,E", &cpu8085::MOV_E_E, &cpu8085::XXX,   1 },
	    { "MOV E,H", &cpu8085::MOV_E_H, &cpu8085::XXX,   1 },
	    { "MOV E,L", &cpu8085::MOV_E_L, &cpu8085::XXX,   1 },
	    { "MOV E,M", &cpu8085::MOV_E_M, &cpu8085::XXX,   1 },
	    { "MOV E,A", &cpu8085::MOV_E_A, &cpu8085::XXX,   1 },
	    { "MOV H,B", &cpu8085::MOV_H_B, &cpu8085::XXX,   1 },
	    { "MOV H,C", &cpu8085::MOV_H_C, &cpu8085::XXX,   1 },
	    { "MOV H,D", &cpu8085::MOV_H_D, &cpu8085::XXX,   1 },
	    { "MOV H,E", &cpu8085::MOV_H_E, &cpu8085::XXX,   1 },
	    { "MOV H,H", &cpu8085::MOV_H_H, &cpu8085::XXX,   1 },
	    { "MOV H,L", &cpu8085::MOV_H_L, &cpu8085::XXX,   1 },
	    { "MOV H,M", &cpu8085::MOV_H_M, &cpu8085::XXX,   1 },
	    { "MOV H,A", &cpu8085::MOV_H_A, &cpu8085::XXX,   1 },
	    { "MOV L,B", &cpu8085::MOV_L_B, &cpu8085::XXX,   1 },
	    { "MOV L,C", &cpu8085::MOV_L_C, &cpu8085::XXX,   1 },
	    { "MOV L,D", &cpu8085::MOV_L_D, &cpu8085::XXX,   1 },
	    { "MOV L,E", &cpu8085::MOV_L_E, &cpu8085::XXX,   1 },
	    { "MOV L,H", &cpu8085::MOV_L_H, &cpu8085::XXX,   1 },
	    { "MOV L,L", &cpu8085::MOV_L_L, &cpu8085::XXX,   1 },
	    { "MOV L,M", &cpu8085::MOV_L_M, &cpu8085::XXX,   1 },
	    { "MOV L,A", &cpu8085::MOV_L_A, &cpu8085::XXX,   1 },
	    { "MOV M,B", &cpu8085::MOV_M_B, &cpu8085::XXX,   1 },
	    { "MOV M,C", &cpu8085::MOV_M_C, &cpu8085::XXX,   1 },
	    { "MOV M,D", &cpu8085::MOV_M_D, &cpu8085::XXX,   1 },
	    { "MOV M,E", &cpu8085::MOV_M_E, &cpu8085::XXX,   1 },
	    { "MOV M,H", &cpu8085::MOV_M_H, &cpu8085::XXX,   1 },
	    { "MOV M,L", &cpu8085::MOV_M_L, &cpu8085::XXX,   1 },
	    { "MOV M,A", &cpu8085::MOV_M_A, &cpu8085::XXX,   1 },
		{ "NOP",     &cpu8085::NOP,    &cpu8085::XXX,    1 },
		{ "MOV A,B", &cpu8085::MOV_A_B, &cpu8085::XXX,   1 },
	    { "MOV A,C", &cpu8085::MOV_A_C, &cpu8085::XXX,   1 },
	    { "MOV A,D", &cpu8085::MOV_A_D, &cpu8085::XXX,   1 },
	    { "MOV A,E", &cpu8085::MOV_A_E, &cpu8085::XXX,   1 },
	    { "MOV A,H", &cpu8085::MOV_A_H, &cpu8085::XXX,   1 },
	    { "MOV A,L", &cpu8085::MOV_A_L, &cpu8085::XXX,   1 },
	    { "MOV A,M", &cpu8085::MOV_A_M, &cpu8085::XXX,   1 },
	    { "MOV A,A", &cpu8085::MOV_A_A, &cpu8085::XXX,   1 },
		{ "ADD B", &cpu8085::ADD_B, &cpu8085::XXX, 1 },
		{ "ADD C", &cpu8085::ADD_C, &cpu8085::XXX, 1 },
		{ "ADD D", &cpu8085::ADD_D, &cpu8085::XXX, 1 },
		{ "ADD E", &cpu8085::ADD_E, &cpu8085::XXX, 1 },
		{ "ADD H", &cpu8085::ADD_H, &cpu8085::XXX, 1 },
		{ "ADD L", &cpu8085::ADD_L, &cpu8085::XXX, 1 },
		{ "ADD M", &cpu8085::ADD_M, &cpu8085::XXX, 1 },
		{ "ADD A", &cpu8085::ADD_A, &cpu8085::XXX, 1 },
		{ "ADC B", &cpu8085::ADC_B, &cpu8085::XXX, 1 },
		{ "ADC C", &cpu8085::ADC_C, &cpu8085::XXX, 1 },
		{ "ADC D", &cpu8085::ADC_D, &cpu8085::XXX, 1 },
		{ "ADC E", &cpu8085::ADC_E, &cpu8085::XXX, 1 },
		{ "ADC H", &cpu8085::ADC_H, &cpu8085::XXX, 1 },
		{ "ADC L", &cpu8085::ADC_L, &cpu8085::XXX, 1 },
		{ "ADC M", &cpu8085::ADC_M, &cpu8085::XXX, 1 },
		{ "ADC A", &cpu8085::ADC_A, &cpu8085::XXX, 1 },
		{ "SUB B", &cpu8085::SUB_B, &cpu8085::XXX, 1 },
		{ "SUB C", &cpu8085::SUB_C, &cpu8085::XXX, 1 },
		{ "SUB D", &cpu8085::SUB_D, &cpu8085::XXX, 1 },
		{ "SUB E", &cpu8085::SUB_E, &cpu8085::XXX, 1 },
		{ "SUB H", &cpu8085::SUB_H, &cpu8085::XXX, 1 },
		{ "SUB L", &cpu8085::SUB_L, &cpu8085::XXX, 1 },
		{ "SUB M", &cpu8085::SUB_M, &cpu8085::XXX, 1 },
		{ "SUB A", &cpu8085::SUB_A, &cpu8085::XXX, 1 },
		{ "SBB B", &cpu8085::SBB_B, &cpu8085::XXX, 1 },
		{ "SBB C", &cpu8085::SBB_C, &cpu8085::XXX, 1 },
		{ "SBB D", &cpu8085::SBB_D, &cpu8085::XXX, 1 },
		{ "SBB E", &cpu8085::SBB_E, &cpu8085::XXX, 1 },
		{ "SBB H", &cpu8085::SBB_H, &cpu8085::XXX, 1 },
		{ "SBB L", &cpu8085::SBB_L, &cpu8085::XXX, 1 },
		{ "SBB M", &cpu8085::SBB_M, &cpu8085::XXX, 1 },
		{ "SBB A", &cpu8085::SBB_A, &cpu8085::XXX, 1 },
		{ "ANA B", &cpu8085::ANA_B, &cpu8085::XXX, 1 },
		{ "ANA C", &cpu8085::ANA_C, &cpu8085::XXX, 1 },
		{ "ANA D", &cpu8085::ANA_D, &cpu8085::XXX, 1 },
		{ "ANA E", &cpu8085::ANA_E, &cpu8085::XXX, 1 },
		{ "ANA H", &cpu8085::ANA_H, &cpu8085::XXX, 1 },
		{ "ANA L", &cpu8085::ANA_L, &cpu8085::XXX, 1 },
		{ "ANA M", &cpu8085::ANA_M, &cpu8085::XXX, 1 },
		{ "ANA A", &cpu8085::ANA_A, &cpu8085::XXX, 1 },
		{ "XRA B", &cpu8085::XRA_B, &cpu8085::XXX, 1 },
		{ "XRA C", &cpu8085::XRA_C, &cpu8085::XXX, 1 },
		{ "XRA D", &cpu8085::XRA_D, &cpu8085::XXX, 1 },
		{ "XRA E", &cpu8085::XRA_E, &cpu8085::XXX, 1 },
		{ "XRA H", &cpu8085::XRA_H, &cpu8085::XXX, 1 },
		{ "XRA L", &cpu8085::XRA_L, &cpu8085::XXX, 1 },
		{ "XRA M", &cpu8085::XRA_M, &cpu8085::XXX, 1 },
		{ "XRA A", &cpu8085::XRA_A, &cpu8085::XXX, 1 },
		{ "ORA B", &cpu8085::ORA_B, &cpu8085::XXX, 1 },
		{ "ORA C", &cpu8085::ORA_C, &cpu8085::XXX, 1 },
		{ "ORA D", &cpu8085::ORA_D, &cpu8085::XXX, 1 },
		{ "ORA E", &cpu8085::ORA_E, &cpu8085::XXX, 1 },
		{ "ORA H", &cpu8085::ORA_H, &cpu8085::XXX, 1 },
		{ "ORA L", &cpu8085::ORA_L, &cpu8085::XXX, 1 },
		{ "ORA M", &cpu8085::ORA_M, &cpu8085::XXX, 1 },
		{ "ORA A", &cpu8085::ORA_A, &cpu8085::XXX, 1 },
		{ "CMP B", &cpu8085::CMP_B, &cpu8085::XXX, 1 },
		{ "CMP C", &cpu8085::CMP_C, &cpu8085::XXX, 1 },
		{ "CMP D", &cpu8085::CMP_D, &cpu8085::XXX, 1 },
		{ "CMP E", &cpu8085::CMP_E, &cpu8085::XXX, 1 },
		{ "CMP H", &cpu8085::CMP_H, &cpu8085::XXX, 1 },
		{ "CMP L", &cpu8085::CMP_L, &cpu8085::XXX, 1 },
		{ "CMP M", &cpu8085::CMP_M, &cpu8085::XXX, 1 },
		{ "CMP A", &cpu8085::CMP_A, &cpu8085::XXX, 1 },
		{ "RNZ", &cpu8085::RNZ, &cpu8085::XXX, 1 },
		{ "POP B", &cpu8085::POP_B, &cpu8085::XXX, 1 },
		{ "JNZ", &cpu8085::JNZ, &cpu8085::IMA, 3 },
		{ "JMP", &cpu8085::JMP, &cpu8085::IMA, 3 },
		{ "CNZ", &cpu8085::CNZ, &cpu8085::IMA, 3 },
		{ "PUSH B", &cpu8085::PUSH_B, &cpu8085::XXX, 1 },
		{ "ADI", &cpu8085::ADI, &cpu8085::IMD_8, 2 },
		{ "RST 0", &cpu8085::RST_0, &cpu8085::XXX, 1 },
		{ "RZ", &cpu8085::RZ, &cpu8085::XXX, 1 },
		{ "RET", &cpu8085::RET, &cpu8085::XXX, 1 },
		{ "JZ", &cpu8085::JZ, &cpu8085::IMA, 3 },
		{ "NOP", &cpu8085::NOP, &cpu8085::XXX, 1 },
		{ "CZ", &cpu8085::CZ, &cpu8085::IMA, 3 },
		{ "CALL", &cpu8085::CALL, &cpu8085::IMA, 3 },
		{ "ACI", &cpu8085::ACI, &cpu8085::IMD_8, 2 },
		{ "RST 1", &cpu8085::RST_1, &cpu8085::XXX, 1 },
		{ "RNC", &cpu8085::RNC, &cpu8085::XXX, 1 },
		{ "POP D", &cpu8085::POP_D, &cpu8085::XXX, 1 },
		{ "JNC", &cpu8085::JNC, &cpu8085::IMA, 3 },
		{ "OUT", &cpu8085::OUT, &cpu8085::IMD_8, 2 },
		{ "CNC", &cpu8085::CNC, &cpu8085::IMA, 3 },
		{ "PUSH D", &cpu8085::PUSH_D, &cpu8085::XXX, 1 },
		{ "SUI", &cpu8085::SUI, &cpu8085::IMD_8, 2 },
		{ "RST 2", &cpu8085::RST_2, &cpu8085::XXX, 1 },
		{ "RC", &cpu8085::RC, &cpu8085::XXX, 1 },
		{ "NOP", &cpu8085::NOP, &cpu8085::XXX, 1 },
		{ "JC", &cpu8085::JC, &cpu8085::IMA, 3 },
		{ "IN", &cpu8085::IN, &cpu8085::IMD_8, 2 },
		{ "CC", &cpu8085::CC, &cpu8085::IMA, 3 },
		{ "NOP", &cpu8085::NOP, &cpu8085::XXX, 1 },
		{ "SBI", &cpu8085::SBI, &cpu8085::IMD_8, 2 },
		{ "RST 3", &cpu8085::RST_3, &cpu8085::XXX, 1 },
		{ "RPO", &cpu8085::RPO, &cpu8085::XXX, 1 },
		{ "POP H", &cpu8085::POP_H, &cpu8085::XXX, 1 },
		{ "JPO", &cpu8085::JPO, &cpu8085::IMA, 3 },
		{ "XTHL", &cpu8085::XTHL, &cpu8085::XXX, 1 },
		{ "CPO", &cpu8085::CPO, &cpu8085::IMA, 3 },
		{ "PUSH H", &cpu8085::PUSH_H, &cpu8085::XXX, 1 },
		{ "ANI", &cpu8085::ANI, &cpu8085::IMD_8, 2 },
		{ "RST 4", &cpu8085::RST_4, &cpu8085::XXX, 1 },
		{ "RPE", &cpu8085::RPE, &cpu8085::XXX, 1 },
		{ "PCHL", &cpu8085::PCHL, &cpu8085::XXX, 1 },
		{ "JPE", &cpu8085::JPE, &cpu8085::IMA, 3 },
		{ "XCHG", &cpu8085::XCHG, &cpu8085::XXX, 1 },
		{ "CPE", &cpu8085::CPE, &cpu8085::IMA, 3 },
		{ "NOP", &cpu8085::NOP, &cpu8085::XXX, 1 },
		{ "XRI", &cpu8085::XRI, &cpu8085::IMD_8, 2 },
		{ "RST 5", &cpu8085::RST_5, &cpu8085::XXX, 1 },
		{ "RP", &cpu8085::RP, &cpu8085::XXX, 1 },
		{ "POP PSW", &cpu8085::POP_PSW, &cpu8085::XXX, 1 },
		{ "JP", &cpu8085::JP, &cpu8085::IMA, 3 },
		{ "DI", &cpu8085::DI, &cpu8085::XXX, 1 },
		{ "CP", &cpu8085::CP, &cpu8085::IMA, 3 },
		{ "PUSH PSW", &cpu8085::PUSH_PSW, &cpu8085::XXX, 1 },
		{ "ORI", &cpu8085::ORI, &cpu8085::IMD_8, 2 },
		{ "RST 6", &cpu8085::RST_6, &cpu8085::XXX, 1 },
		{ "RM", &cpu8085::RM, &cpu8085::XXX, 1 },
		{ "SPHL", &cpu8085::SPHL, &cpu8085::XXX, 1 },
		{ "JM", &cpu8085::JM, &cpu8085::IMA, 3 },
		{ "EI", &cpu8085::EI, &cpu8085::XXX, 1 },
		{ "CM", &cpu8085::CM, &cpu8085::IMA, 3 },
		{ "NOP", &cpu8085::NOP, &cpu8085::XXX, 1 },
		{ "CPI", &cpu8085::CPI, &cpu8085::IMD_8, 2 },
		{ "RST 7", &cpu8085::RST_7, &cpu8085::XXX, 1 }
	};
}

cpu8085::~cpu8085()
{
	// Destructor - has nothing to do
}


///////////////////////////////////////////////////////////////////////////////
// BUS CONNECTIVITY

// Reads an 8-bit byte from the bus, located at the specified 16-bit address
uint8_t cpu8085::read(uint16_t a)
{
	return bus->read(a, false); // false => read only
}

// Writes a byte to the bus at the specified address
void cpu8085::write(uint16_t a, uint8_t d)
{
	bus->write(a, d);
}


// Perform one clock cycles worth of emulation
void cpu8085::clock()
{

	if (cycles == 0) // when cycle =0 execute in one go
	{
		// Read next instruction byte. This 8-bit value is used to index
		// the translation table to get the relevant information about
		// how to implement the instruction
		opcode = read(pc);

		// Increment program counter, we read the opcode byte
		pc++;

		// Get Starting number of cycles
		cycles = lookup[opcode].cycles;

		// Perform fetch of intermmediate data using the
		// required addressing mode
		uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();

		// Perform operation [function call to Instrs]
		uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

		// The addressmode and opcode may have altered the number
		// of cycles this instruction requires before its completed
		cycles += (additional_cycle1 & additional_cycle2);


	}

	// Increment global clock count - This is actually unused unless logging is enabled
	// but I've kept it in because its a handy watch variable for debugging
	clock_count++;

	// Decrement the number of cycles remaining for this instruction
	cycles--;
}





///////////////////////////////////////////////////////////////////////////////
// FLAG FUNCTIONS

// Returns the value of a specific bit of the status register
uint8_t cpu8085::GetFlag(FLAGS8085 f)
{
	return ((status & f) > 0) ? 1 : 0;
}

// Sets or clears a specific bit of the status register
void cpu8085::SetFlag(FLAGS8085 f, bool v)
{
	if (v)
		status |= f;
	else
		status &= ~f;
}


///////////////////////////////////////////////////////////////////////////////
// ADDRESSING MODES (when these func are called pc points to addr next to opcode)
	// return extra cycles
	uint8_t cpu8085::IMD_8()  // immediate data 8 bit Eg: MVI C 15h => fetched_low =15 (used by MVI_C())
	{
		fetched_low = read(pc);
		pc++;
		return 0;
	}

	uint8_t cpu8085::IMD_16() // immediate data 16 bit Eg: LXI B 1000h => fetched_low = 00 , fetched_high =10;
	{
		// follow litte endian
		fetched_low = read(pc);
		pc++;
		fetched_high = read(pc);
		pc++;
		return 0;
	}

	uint8_t cpu8085::IMA() //immediate address Eg : LDA 1000h => addr_abs = 1000h
	{
		uint8_t addr_low = read(pc); pc++;
		uint8_t addr_high = read(pc); pc++;
		addr_abs = ((uint16_t)addr_high << 8) | addr_low;
		return 0;
	}

///////////////////////////////////////////////////////////////////////////////
// INSTRUCTION IMPLEMENTATIONS

	uint8_t cpu8085::NOP(){
		return 0;
	}
	/*
	LXI B 2350h => B = 23 C = 50
	As has IMD_16 => fetched_low =50 and fetched_high = 23
	*/
	uint8_t cpu8085::LXI_B(){
		b = fetched_high;
		c = fetched_low;
		return 0;
	}


///////////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS

bool cpu8085::complete()
{
	return cycles == 0;
}

// This is the disassembly function.
std::map<uint16_t, std::string> cpu8085::disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into
	// hex strings because "modern C++"'s method with
	// streams is atrocious
	auto hex = [](uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the lookup table
	// as to how many additional bytes we need to read and what the
	// addressing mode is. I need this info to assemble human readable
	// syntax, which is different depending upon the addressing mode

	// As the instruction is decoded, a std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address (eg $8000 :)// in hex
		std::string sInst = "$" + hex(addr, 4) + ": "; // 4 size of string(hex)

		// Read instruction, and get its readable name
		uint8_t opcode = bus->read(addr, true); addr++;
		sInst += lookup[opcode].name + " ";

		/*
		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 8085 in order to get accurate data as part of the
		// instruction
		if (lookup[opcode].addrmode == &cpu8085::IMP)
		{
			sInst += " {IMP}";
		}
		else if (lookup[opcode].addrmode == &cpu8085::IMM)
		{
			value = bus->read(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (lookup[opcode].addrmode == &cpu8085::ZP0)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (lookup[opcode].addrmode == &cpu8085::ZPX)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (lookup[opcode].addrmode == &cpu8085::ZPY)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (lookup[opcode].addrmode == &cpu8085::IZX)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (lookup[opcode].addrmode == &cpu8085::IZY)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (lookup[opcode].addrmode == &cpu8085::ABS)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (lookup[opcode].addrmode == &cpu8085::ABX)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (lookup[opcode].addrmode == &cpu8085::ABY)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (lookup[opcode].addrmode == &cpu8085::IND)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (lookup[opcode].addrmode == &cpu8085::REL)
		{
			value = bus->read(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "] {REL}";
		}
		*/
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}


