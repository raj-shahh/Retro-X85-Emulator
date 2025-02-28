
#include <iostream>
#include <sstream>

#include "Bus.h"
#include "cpu8085.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"



class Emulate_cpu8085 : public olc::PixelGameEngine
{
public:
	Emulate_cpu8085() { sAppName = "Intel 8085 Emulation"; }

	Bus emu_bus;
	std::map<uint16_t, std::string> mapAsm; // map assembly

	// Utility function to convert to hex
	std::string hex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns)
	{
		int nRamX = x, nRamY = y;
		for (int row = 0; row < nRows; row++)
		{
			std::string sOffset = "$" + hex(nAddr, 4) + ":";
			for (int col = 0; col < nColumns; col++)
			{
				sOffset += " " + hex(emu_bus.read(nAddr, true), 2);
				nAddr += 1;
			}
			DrawString(nRamX, nRamY, sOffset);
			nRamY += 10;
		}
	}

	void DrawCpu(int x, int y)
	{
		//std::string status = "STATUS: ";
		DrawString(x , y , "STATUS:", olc::WHITE);
		DrawString(x  + 64, y, "N", emu_bus.cpu.status & cpu8085::N ? olc::GREEN : olc::RED);
		DrawString(x  + 80, y , "V", emu_bus.cpu.status & cpu8085::V ? olc::GREEN : olc::RED);
		DrawString(x  + 96, y , "-", emu_bus.cpu.status & cpu8085::U ? olc::GREEN : olc::RED);
		DrawString(x  + 112, y , "B", emu_bus.cpu.status & cpu8085::B ? olc::GREEN : olc::RED);
		DrawString(x  + 128, y , "D", emu_bus.cpu.status & cpu8085::D ? olc::GREEN : olc::RED);
		DrawString(x  + 144, y , "I", emu_bus.cpu.status & cpu8085::I ? olc::GREEN : olc::RED);
		DrawString(x  + 160, y , "Z", emu_bus.cpu.status & cpu8085::Z ? olc::GREEN : olc::RED);
		DrawString(x  + 178, y , "C", emu_bus.cpu.status & cpu8085::C ? olc::GREEN : olc::RED);
		DrawString(x , y + 10, "PC: $" + hex(emu_bus.cpu.pc, 4));
		DrawString(x , y + 20, "A: $" +  hex(emu_bus.cpu.a, 2) + "  [" + std::to_string(emu_bus.cpu.a) + "]");
		DrawString(x , y + 30, "X: $" +  hex(emu_bus.cpu.x, 2) + "  [" + std::to_string(emu_bus.cpu.x) + "]");
		DrawString(x , y + 40, "Y: $" +  hex(emu_bus.cpu.y, 2) + "  [" + std::to_string(emu_bus.cpu.y) + "]");
		DrawString(x , y + 50, "Stack P: $" + hex(emu_bus.cpu.stkp, 4));
	}

	void DrawCode(int x, int y, int nLiemu_bus)
	{
		auto it_a = mapAsm.find(emu_bus.cpu.pc);
		int nLineY = (nLiemu_bus >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			DrawString(x, nLineY, (*it_a).second, olc::CYAN);
			while (nLineY < (nLiemu_bus * 10) + y)
			{
				nLineY += 10;
				if (++it_a != mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second);
				}
			}
		}

		it_a = mapAsm.find(emu_bus.cpu.pc);
		nLineY = (nLiemu_bus >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			while (nLineY > y)
			{
				nLineY -= 10;
				if (--it_a != mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second);
				}
			}
		}
	}

	bool OnUserCreate()
	{
		// Load Program (assembled at https://www.masswerk.at/6502/assembler.html)
		/*
			*=$8000
			LDX #10
			STX $0000
			LDX #3
			STX $0001
			LDY $0000
			LDA #0
			CLC
			loop
			ADC $0001
			DEY
			BNE loop
			STA $0002
			NOP
			NOP
			NOP
		*/
		
		// Convert hex string into bytes for RAM
		std::stringstream ss;
		ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
		uint16_t nOffset = 0x8000; // start address of code
		while (!ss.eof())
		{
			std::string b;
			ss >> b;
			emu_bus.ram[nOffset++] = (uint8_t)std::stoul(b, nullptr, 16); // store in Ram
		}

		// Set Reset Vector
		emu_bus.ram[0xFFFC] = 0x00;
		emu_bus.ram[0xFFFD] = 0x80;

		// Dont forget to set IRQ and NMI vectors if you want to play with those
				
		// Extract dissassembly
		mapAsm = emu_bus.cpu.disassemble(0x0000, 0xFFFF);

		// Reset
		emu_bus.cpu.reset();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		Clear(olc::DARK_BLUE);


		if (GetKey(olc::Key::SPACE).bPressed) // if space bar key is pressed
		{
			do
			{
				emu_bus.cpu.clock();
			} 
			while (!emu_bus.cpu.complete());
		}

		if (GetKey(olc::Key::R).bPressed)
			emu_bus.cpu.reset();

		if (GetKey(olc::Key::I).bPressed)
			emu_bus.cpu.irq();

		if (GetKey(olc::Key::N).bPressed)
			emu_bus.cpu.nmi();

		/*
		16 col => Each col has 1 byte (8 bit) of data corresponding to 1 address(16 bit)
		==> 1 st row address displayed = 0000h
		==> 2nd row address displayed = 0010h (10h = 16 in dec)
		==> 1st row 1st col data is corresponding to address 0001h
		*/				
		DrawRam(2, 2, 0x0000, 16, 16); // draws start Adress = 0000h and 16 rows and 16 col
		DrawRam(2, 182, 0x8000, 16, 16); // 1st 2 params (2,182) position on screen where to start draw
		DrawCpu(448, 2);
		DrawCode(448, 72, 26);

		//IRQ : Interrupt Request  NMI: Non Maskable Interrupt
		DrawString(10, 370, "SPACE = Step Instruction    R = RESET    I = IRQ    N = NMI");

		return true;
	}
};



/*					Working
When create pge(main()) --> creates Bus obj(emu_bus) as data member
When emu_bus is created creates cpu obj ( as data member) [init pc]
As we press space bar -> exe cpu.clock(); // OnUSerUpdate (exe every frame)
clock() --> executes only 1 instruction
** The code to be executed is Stored in Ram [OnUSerCreate (exe Once)]
*/

int main()
{
	Emulate_cpu8085 pge; //pixel game engine
	if(pge.Construct(680, 480, 2, 2))
		pge.Start();
	return 0;
}