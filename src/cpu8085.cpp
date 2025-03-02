
#include "cpu8085.h"
#include "Bus.h"

// Constructor
cpu8085::cpu8085()
{


	// The table is one big initialiser list of initialiser lists...
	using a = cpu8085;
	lookup = 
	{
		{ "NOP", &a::NOP, &a::XXX, 1 } , {"LXI B", &a::LXI_B, &a::IMD_16, 3},
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


