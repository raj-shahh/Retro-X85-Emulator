#include "cpu8085.h"
#include "Bus.h"

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
