
#include "cpu8085.h"
#include "Bus.h"

//Forces CPU into known state
void cpu8085::reset(uint16_t progStartAddress){

	pc = progStartAddress;
	stkp = 0xFFFF;

	a = 0x00; // A Accumulator Register
    b = 0x00; // B Register  (B-C) pair
    c = 0x00; // C Register
    d = 0x00; // D Register  (D-E) pair
    e = 0x00; // E Register
    h = 0x00; // H Register  (H-L) pair
    l = 0x00; // L Register
    status = 0x00; // Status Register [F] flag

	// internal variables
	fetched_low = 0x00; // MVI C 15 => fectched_low =15
    fetched_high = 0x00; // LXI B 1000 => fetched_low = 00 ; fetched_high= 10
    addr_abs = 0x0000; // LDA 1000h => addr_abs =1000
    opcode = 0x00; // Is the instruction byte
    cycles = 0; // Counts how many cycles the instruction has remaining
    clock_count = 0; // A global accumulation of the number of clocks

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

// check if current instr complete?
bool cpu8085::complete()
{
	return cycles == 0;
}

// Link this CPU to a communications bus
void cpu8085::ConnectBus(Bus *busptr) { 
	bus = busptr; 
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

/////////////////////////////////////////////////////////////////////////////////////////////

// This is the disassembly function.
std::map<uint16_t, std::string> cpu8085::disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t  lo = 0x00, hi = 0x00;
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
	/* # means data $ means address
	mapLines => key = $8000  val = LDA $1000h
				key = $8003  val = INR A 
	*/
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address (eg $8000 :)// in hex
		std::string sInst = "$" + hex(addr, 4) + ": "; // 4 size of string(hex)

		// Read instruction, and get its readable name
		uint8_t opcode = bus->read(addr, true); addr++;
		sInst += lookup[opcode].name + " ";

		// Get oprands from desired locations, and form the instruction based upon its addressing mode. 
		if (lookup[opcode].addrmode == &cpu8085::IMD_8)
		{
			lo = bus->read(addr, true); addr++;
			sInst += "#" + hex(lo, 2)+"h";			
		}
		else if (lookup[opcode].addrmode == &cpu8085::IMD_16)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "#" + hex((uint16_t)(hi << 8) | lo, 4)+"h";
		}
		else if (lookup[opcode].addrmode == &cpu8085::IMA)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4)+"h";
		}
		else{
			sInst += " {IMP}";
		}

		mapLines[line_addr] = sInst;
	}

	return mapLines;
}


