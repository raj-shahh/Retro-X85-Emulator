
#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <map>



// Forward declaration of generic communications bus class to
// prevent circular inclusions(
/*
Cpu needs Bus * ; Forwd declare class Bus;
Bus needs Cpu obj: #include cpu.h (full definition)
*/

class Bus;


// The Emulation Class.
class cpu8085
{
public:
	cpu8085();
	~cpu8085();

public:
	// CPU Core registers,
	uint8_t  a      = 0x00;		// A Accumulator Register
	uint8_t  b      = 0x00;		// B Register  (B-C) pair
	uint8_t  c      = 0x00;		// C Register
	uint8_t  d      = 0x00;		// D Register  (D-E) pair
	uint8_t  e      = 0x00;		// E Register	
	uint8_t  h      = 0x00;		// H Register  (H-L) pair
	uint8_t  l      = 0x00;		// L Register
	uint8_t  status = 0x00;		// Status Register [F] flag
	uint16_t stkp   = 0x0000;	// Stack Pointer
	uint16_t pc     = 0x0000;	// Program Counter
	
	

	void reset();	// Reset Interrupt - Forces CPU into known state
	void clock();	// Perform one clock cycle's worth of update

	// Indicates the current instruction has completed by returning true
	bool complete();

	// Link this CPU to a communications bus
	void ConnectBus(Bus * busptr) { bus = busptr; }

	std::map<uint16_t, std::string> disassemble(uint16_t nStart, uint16_t nStop);

	// The status register stores 8 flags.
	enum FLAGS8085
	{
		C = (1 << 0),	// Carry Bit
		P = (1 << 2),	// Parity
		A = (1 << 4),	// Auxillary Carry
		Z = (1 << 6),	// Zero
		S = (1 << 7),	// Sign
	};

private:
	// Convenience functions to access status register
	uint8_t GetFlag(FLAGS8085 f);
	void    SetFlag(FLAGS8085 f, bool v);
	
	// Assisstive variables to facilitate emulation
	uint8_t  fetched_low     = 0x00;   // MVI C 15 => fectched_low =15
	uint8_t  fetched_high    = 0x00; // LXI B 1000 => fetched_low = 00 ; fetched_high= 10
	uint16_t addr_abs    = 0x0000; // LDA 1000h => addr_abs =1000
	uint8_t  opcode      = 0x00;   // Is the instruction byte
	uint8_t  cycles      = 0;	   // Counts how many cycles the instruction has remaining
	uint32_t clock_count = 0;	   // A global accumulation of the number of clocks

	// Linkage to the communications bus
	Bus     *bus = nullptr;
	uint8_t read(uint16_t a);
	void    write(uint16_t a, uint8_t d);


	// Each table(lookup -vector) entry holds:
	//	Pneumonic : A textual representation of the instruction (used for disassembly)
	//	Opcode Function: A function pointer to the implementation of the opcode
	//	Opcode Address Mode : A function pointer to the implementation of the 
    //						  addressing mechanism used by the instruction
	//	Cycle Count : An integer that represents the base number of clock cycles the
	//				  CPU requires to perform the instruction

	struct INSTRUCTION
	{
		std::string name;		
		uint8_t     (cpu8085::*operate )(void) = nullptr;
		uint8_t     (cpu8085::*addrmode)(void) = nullptr;
		uint8_t     cycles = 0;
	};

	std::vector<INSTRUCTION> lookup;
	
private: 
	// Addressing Modes =============================================
	// Manipulates fetched_low(data), fetched_high(data) and addr_abs(address) variables

	uint8_t IMD_8();  // immediate data 8 bit Eg: MVI C 15h => fetched_low =15 (used by MVI_C())
	uint8_t IMD_16(); // immediate data 16 bit Eg: LXI B 1000h => fetched_low = 00 , fetched_high =10;
	uint8_t IMA(); //immediate address Eg : LDA 1000h => addr_abs = 1000h

	// for rest addressing modes concern register call XXX() and handle main logic in the instruction itself

private: 
	// Opcodes ===================INstructions===================================

	// All return type uint8_t

	uint8_t NOP();// opcode 00
	uint8_t LXI_B();// opcode 01...

	//capture illegal opcode and Dummy addressing Modes
	uint8_t XXX(){
		return 0;
	}

};

