#pragma once
#include <cstdint>
#include <array>

#include "cpu8085.h" // full definition of cpu as bus needs cpu object[data member]

class Bus
{
public:
	Bus();
	~Bus();

public: // Devices on bus
	cpu8085 cpu;	

	// Fake RAM for this part of the series
	std::array<uint8_t, 64 * 1024> ram;


public: // Bus Read & Write
	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr, bool bReadOnly = false);
};

