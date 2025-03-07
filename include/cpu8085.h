#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <map>


// Forward declaration of generic communications bus class to prevent circular inclusions
//Cpu needs Bus * ; Forwd declare class Bus;
//Bus needs Cpu obj: #include cpu.h (full definition)
class Bus;


// The Emulation Class.
class cpu8085 {
public:
    cpu8085();

    ~cpu8085();

public:  // Public Data Members ...........
    // CPU Core registers,
    uint8_t a = 0x00; // A Accumulator Register
    uint8_t b = 0x00; // B Register  (B-C) pair
    uint8_t c = 0x00; // C Register
    uint8_t d = 0x00; // D Register  (D-E) pair
    uint8_t e = 0x00; // E Register
    uint8_t h = 0x00; // H Register  (H-L) pair
    uint8_t l = 0x00; // L Register
    uint8_t status = 0x00; // Status Register [F] flag
    uint16_t stkp = 0x0000; // Stack Pointer
    uint16_t pc = 0x0000; // Program Counter

    // The status register stores 8 flags.
    enum FLAGS8085 {
        C = (1 << 0), // Carry Bit
        P = (1 << 2), // Parity
        A = (1 << 4), // Auxillary Carry
        Z = (1 << 6), // Zero
        S = (1 << 7), // Sign
    };

    bool stop_exe_flag = false; // set to true only when exe NOP/HLT/RST

public: // Member functions
    // User Side Interface ...

    void reset(uint16_t progStartAddress); // Reset Interrupt - Forces CPU into known state
    void clock(); // Perform one clock cycle's worth of update

    // Indicates the current instruction has completed by returning true
    bool complete();

    // Link this CPU to a communications bus
    void ConnectBus(Bus *busptr);

    // Convenience functions to access status register
    uint8_t GetFlag(FLAGS8085 f);

    void SetFlag(FLAGS8085 f, bool v);

    void allSetFlags(uint16_t tempComputation, bool aux_flag_cond, bool setCarry = true) {
        if (setCarry) SetFlag(FLAGS8085::C, tempComputation > 0xFF);
        SetFlag(FLAGS8085::Z, (tempComputation & 0xFF) == 0);
        SetFlag(FLAGS8085::S, tempComputation & 0x80);
        SetFlag(FLAGS8085::P, __builtin_popcount(tempComputation & 0xFF) & 0x01 == 0);
        SetFlag(FLAGS8085::A, aux_flag_cond);
    }

    std::map<uint16_t, std::string> disassemble(uint16_t nStart, uint16_t nStop);

protected:
    ////////////  Utility Data Members /////////////////////////

    // Assisstive variables to facilitate emulation
    uint8_t fetched_low = 0x00; // MVI C 15 => fectched_low =15
    uint8_t fetched_high = 0x00; // LXI B 1000 => fetched_low = 00 ; fetched_high= 10
    uint16_t addr_abs = 0x0000; // LDA 1000h => addr_abs =1000
    uint8_t opcode = 0x00; // Is the instruction byte
    uint8_t cycles = 0; // Counts how many cycles the instruction has remaining
    uint32_t clock_count = 0; // A global accumulation of the number of clocks

    // Linkage to the communications bus
    Bus *bus = nullptr;

    // Each table(lookup -vector) entry holds:
    //	Pneumonic : A textual representation of the instruction (used for disassembly)
    //	Opcode Function: A function pointer to the implementation of the opcode
    //	Opcode Address Mode : A function pointer to the implementation of the
    //						  addressing mechanism used by the instruction
    //	Cycle Count : An integer that represents the base number of clock cycles the
    //				  CPU requires to perform the instruction

struct INSTRUCTION {
    std::string name;
    uint8_t (cpu8085::*operate)(void) = nullptr;
    uint8_t (cpu8085::*addrmode)(void) = nullptr;
    uint8_t cycles = 0;

    // Constructor for easy initialization
    INSTRUCTION(const std::string& n, uint8_t (cpu8085::*op)(), uint8_t (cpu8085::*m)(), uint8_t c)
        : name(n), operate(op), addrmode(m), cycles(c) {}
};


    std::vector<INSTRUCTION> lookup;

protected:
    //Utiltiy Functions .......................

    uint8_t read(uint16_t a);

    void write(uint16_t a, uint8_t d);

    uint8_t RST_helper(uint8_t n);

protected:
    // Addressing Modes =============================================
    // Manipulates fetched_low(data), fetched_high(data) and addr_abs(address) variables

    uint8_t IMD_8(); // immediate data 8 bit Eg: MVI C 15h => fetched_low =15 (used by MVI_C())
    uint8_t IMD_16(); // immediate data 16 bit Eg: LXI B 1000h => fetched_low = 00 , fetched_high =10;
    uint8_t IMA(); //immediate address Eg : LDA 1000h => addr_abs = 1000h

    // for rest addressing modes concern register call XXX() and handle main logic in the instruction itself

protected:
    // Opcodes ===================INstructions===================================

    // All return type uint8_t [extra cycles required]

    uint8_t NOP();      // opcode 00
    uint8_t LXI_B();    // opcode 01
    uint8_t STAX_B();   // opcode 02
    uint8_t INX_B();    // opcode 03
    uint8_t INR_B();    // opcode 04
    uint8_t DCR_B(); // opcode 05
    uint8_t MVI_B(); // opcode 06
    uint8_t RLC(); // opcode 07
    uint8_t DAD_B(); // opcode 09
    uint8_t LDAX_B(); // opcode 0A
    uint8_t DCX_B(); // opcode 0B
    uint8_t INR_C(); // opcode 0C
    uint8_t DCR_C(); // opcode 0D
    uint8_t MVI_C(); // opcode 0E
    uint8_t RRC(); // opcode 0F
    uint8_t LXI_D(); // opcode 11
    uint8_t STAX_D(); // opcode 12
    uint8_t INX_D(); // opcode 13
    uint8_t INR_D(); // opcode 14
    uint8_t DCR_D(); // opcode 15
    uint8_t MVI_D(); // opcode 16
    uint8_t RAL(); // opcode 17
    uint8_t DAD_D(); // opcode 19
    uint8_t LDAX_D(); // opcode 1A
    uint8_t DCX_D(); // opcode 1B
    uint8_t INR_E(); // opcode 1C
    uint8_t DCR_E(); // opcode 1D
    uint8_t MVI_E(); // opcode 1E
    uint8_t RAR(); // opcode 1F
    uint8_t RIM(); // opcode 20
    uint8_t LXI_H(); // opcode 21
    uint8_t SHLD(); // opcode 22
    uint8_t INX_H(); // opcode 23
    uint8_t INR_H(); // opcode 24
    uint8_t DCR_H(); // opcode 25
    uint8_t MVI_H(); // opcode 26
    uint8_t DAA(); // opcode 27
    uint8_t DAD_H(); // opcode 29
    uint8_t LHLD(); // opcode 2A
    uint8_t DCX_H(); // opcode 2B
    uint8_t INR_L(); // opcode 2C
    uint8_t DCR_L(); // opcode 2D
    uint8_t MVI_L(); // opcode 2E
    uint8_t CMA(); // opcode 2F
    uint8_t SIM(); // opcode 30
    uint8_t LXI_SP(); // opcode 31
    uint8_t STA(); // opcode 32
    uint8_t INX_SP(); // opcode 33
    uint8_t INR_M(); // opcode 34
    uint8_t DCR_M(); // opcode 35
    uint8_t MVI_M(); // opcode 36
    uint8_t STC(); // opcode 37
    uint8_t DAD_SP(); // opcode 39
    uint8_t LDA(); // opcode 3A
    uint8_t DCX_SP(); // opcode 3B
    uint8_t INR_A(); // opcode 3C
    uint8_t DCR_A(); // opcode 3D
    uint8_t MVI_A(); // opcode 3E
    uint8_t CMC(); // opcode 3F
    uint8_t MOV_B_B(); // opcode 40
    uint8_t MOV_B_C(); // opcode 41
    uint8_t MOV_B_D(); // opcode 42
    uint8_t MOV_B_E(); // opcode 43
    uint8_t MOV_B_H(); // opcode 44
    uint8_t MOV_B_L(); // opcode 45
    uint8_t MOV_B_M(); // opcode 46
    uint8_t MOV_B_A(); // opcode 47
    uint8_t MOV_C_B(); // opcode 48
    uint8_t MOV_C_C(); // opcode 49
    uint8_t MOV_C_D(); // opcode 4A
    uint8_t MOV_C_E(); // opcode 4B
    uint8_t MOV_C_H(); // opcode 4C
    uint8_t MOV_C_L(); // opcode 4D
    uint8_t MOV_C_M(); // opcode 4E
    uint8_t MOV_C_A(); // opcode 4F
    uint8_t MOV_D_B(); // opcode 50
    uint8_t MOV_D_C(); // opcode 51
    uint8_t MOV_D_D(); // opcode 52
    uint8_t MOV_D_E(); // opcode 53
    uint8_t MOV_D_H(); // opcode 54
    uint8_t MOV_D_L(); // opcode 55
    uint8_t MOV_D_M(); // opcode 56
    uint8_t MOV_D_A(); // opcode 57
    uint8_t MOV_E_B(); // opcode 58
    uint8_t MOV_E_C(); // opcode 59
    uint8_t MOV_E_D(); // opcode 5A
    uint8_t MOV_E_E(); // opcode 5B
    uint8_t MOV_E_H(); // opcode 5C
    uint8_t MOV_E_L(); // opcode 5D
    uint8_t MOV_E_M(); // opcode 5E
    uint8_t MOV_E_A(); // opcode 5F
    uint8_t MOV_H_B(); // opcode 60
    uint8_t MOV_H_C(); // opcode 61
    uint8_t MOV_H_D(); // opcode 62
    uint8_t MOV_H_E(); // opcode 63
    uint8_t MOV_H_H(); // opcode 64
    uint8_t MOV_H_L(); // opcode 65
    uint8_t MOV_H_M(); // opcode 66
    uint8_t MOV_H_A(); // opcode 67
    uint8_t MOV_L_B(); // opcode 68
    uint8_t MOV_L_C(); // opcode 69
    uint8_t MOV_L_D(); // opcode 6A
    uint8_t MOV_L_E(); // opcode 6B
    uint8_t MOV_L_H(); // opcode 6C
    uint8_t MOV_L_L(); // opcode 6D
    uint8_t MOV_L_M(); // opcode 6E
    uint8_t MOV_L_A(); // opcode 6F
    uint8_t MOV_M_B(); // opcode 70
    uint8_t MOV_M_C(); // opcode 71
    uint8_t MOV_M_D(); // opcode 72
    uint8_t MOV_M_E(); // opcode 73
    uint8_t MOV_M_H(); // opcode 74
    uint8_t MOV_M_L(); // opcode 75
    uint8_t HLT(); // opcode 76
    uint8_t MOV_M_A(); // opcode 77
    uint8_t MOV_A_B(); // opcode 78
    uint8_t MOV_A_C(); // opcode 79
    uint8_t MOV_A_D(); // opcode 7A
    uint8_t MOV_A_E(); // opcode 7B
    uint8_t MOV_A_H(); // opcode 7C
    uint8_t MOV_A_L(); // opcode 7D
    uint8_t MOV_A_M(); // opcode 7E
    uint8_t MOV_A_A(); // opcode 7F
    uint8_t ADD_B(); // opcode 80
    uint8_t ADD_C(); // opcode 81
    uint8_t ADD_D(); // opcode 82
    uint8_t ADD_E(); // opcode 83
    uint8_t ADD_H(); // opcode 84
    uint8_t ADD_L(); // opcode 85
    uint8_t ADD_M(); // opcode 86
    uint8_t ADD_A(); // opcode 87
    uint8_t ADC_B(); // opcode 88
    uint8_t ADC_C(); // opcode 89
    uint8_t ADC_D(); // opcode 8A
    uint8_t ADC_E(); // opcode 8B
    uint8_t ADC_H(); // opcode 8C
    uint8_t ADC_L(); // opcode 8D
    uint8_t ADC_M(); // opcode 8E
    uint8_t ADC_A(); // opcode 8F
    uint8_t SUB_B(); // opcode 90
    uint8_t SUB_C(); // opcode 91
    uint8_t SUB_D(); // opcode 92
    uint8_t SUB_E(); // opcode 93
    uint8_t SUB_H(); // opcode 94
    uint8_t SUB_L(); // opcode 95
    uint8_t SUB_M(); // opcode 96
    uint8_t SUB_A(); // opcode 97
    uint8_t SBB_B(); // opcode 98
    uint8_t SBB_C(); // opcode 99
    uint8_t SBB_D(); // opcode 9A
    uint8_t SBB_E(); // opcode 9B
    uint8_t SBB_H(); // opcode 9C
    uint8_t SBB_L(); // opcode 9D
    uint8_t SBB_M(); // opcode 9E
    uint8_t SBB_A(); // opcode 9F
    uint8_t ANA_B(); // opcode A0
    uint8_t ANA_C(); // opcode A1
    uint8_t ANA_D(); // opcode A2
    uint8_t ANA_E(); // opcode A3
    uint8_t ANA_H(); // opcode A4
    uint8_t ANA_L(); // opcode A5
    uint8_t ANA_M(); // opcode A6
    uint8_t ANA_A(); // opcode A7
    uint8_t XRA_B(); // opcode A8
    uint8_t XRA_C(); // opcode A9
    uint8_t XRA_D(); // opcode AA
    uint8_t XRA_E(); // opcode AB
    uint8_t XRA_H(); // opcode AC
    uint8_t XRA_L(); // opcode AD
    uint8_t XRA_M(); // opcode AE
    uint8_t XRA_A(); // opcode AF
    uint8_t ORA_B(); // opcode B0
    uint8_t ORA_C(); // opcode B1
    uint8_t ORA_D(); // opcode B2
    uint8_t ORA_E(); // opcode B3
    uint8_t ORA_H(); // opcode B4
    uint8_t ORA_L(); // opcode B5
    uint8_t ORA_M(); // opcode B6
    uint8_t ORA_A(); // opcode B7
    uint8_t CMP_B(); // opcode B8
    uint8_t CMP_C(); // opcode B9
    uint8_t CMP_D(); // opcode BA
    uint8_t CMP_E(); // opcode BB
    uint8_t CMP_H(); // opcode BC
    uint8_t CMP_L(); // opcode BD
    uint8_t CMP_M(); // opcode BE
    uint8_t CMP_A(); // opcode BF
    uint8_t RNZ(); // opcode C0
    uint8_t POP_B(); // opcode C1
    uint8_t JNZ(); // opcode C2
    uint8_t JMP(); // opcode C3
    uint8_t CNZ(); // opcode C4
    uint8_t PUSH_B(); // opcode C5
    uint8_t ADI(); // opcode C6
    uint8_t RST_0(); // opcode C7
    uint8_t RZ(); // opcode C8
    uint8_t RET(); // opcode C9
    uint8_t JZ(); // opcode CA
    uint8_t CZ(); // opcode CC
    uint8_t CALL(); // opcode CD
    uint8_t ACI(); // opcode CE
    uint8_t RST_1(); // opcode CF
    uint8_t RNC(); // opcode D0
    uint8_t POP_D(); // opcode D1
    uint8_t JNC(); // opcode D2
    uint8_t OUT(); // opcode D3
    uint8_t CNC(); // opcode D4
    uint8_t PUSH_D(); // opcode D5
    uint8_t SUI(); // opcode D6
    uint8_t RST_2(); // opcode D7
    uint8_t RC(); // opcode D8
    uint8_t JC(); // opcode DA
    uint8_t IN(); // opcode DB
    uint8_t CC(); // opcode DC
    uint8_t SBI(); // opcode DE
    uint8_t RST_3(); // opcode DF
    uint8_t RPO(); // opcode E0
    uint8_t POP_H(); // opcode E1
    uint8_t JPO(); // opcode E2
    uint8_t XTHL(); // opcode E3
    uint8_t CPO(); // opcode E4
    uint8_t PUSH_H(); // opcode E5
    uint8_t ANI(); // opcode E6
    uint8_t RST_4(); // opcode E7
    uint8_t RPE(); // opcode E8
    uint8_t PCHL(); // opcode E9
    uint8_t JPE(); // opcode EA
    uint8_t XCHG(); // opcode EB
    uint8_t CPE(); // opcode EC
    uint8_t XRI(); // opcode EE
    uint8_t RST_5(); // opcode EF
    uint8_t RP(); // opcode F0
    uint8_t POP_PSW(); // opcode F1
    uint8_t JP();       // opcode F2
    uint8_t DI(); // opcode F3
    uint8_t CP(); // opcode F4
    uint8_t PUSH_PSW(); // opcode F5
    uint8_t ORI(); // opcode F6
    uint8_t RST_6(); // opcode F7
    uint8_t RM(); // opcode F8
    uint8_t SPHL(); // opcode F9
    uint8_t JM(); // opcode FA
    uint8_t EI();       // opcode FB
    uint8_t CM();       // opcode FC
    uint8_t CPI();      // opcode FE
    uint8_t RST_7();    // opcode FF

    //capture illegal opcode and Dummy addressing Modes
    uint8_t XXX() {
        return 0;
    }
};
