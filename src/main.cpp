
#include <iostream>
#include <sstream>

#include "Bus.h"
#include "cpu8085.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "assembler.h"



class Emulate_cpu8085 : public olc::PixelGameEngine
{
public:
	Emulate_cpu8085() { sAppName = "Intel 8085 Emulation"; }

	Bus emu_bus;
	std::map<uint16_t, std::string> mapAsm; // map assembly
	uint16_t progStartAddress;
	std::string progFilePath;
	uint16_t y_offset = 50; 
	bool inputMode = false;          // Flag to check if input mode is active
	std::string userInput = "";      // Stores user-entered address
	uint16_t userRamAddress = 0x0000; // Default RAM display address
	bool firstKeyIgnored = false; // NEW: Track first keypress
	bool fullExecutionMode = false;

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
		uint16_t maxAddr = 0xFFFF;
		bool flag = false;

		for (int row = 0; row < nRows; row++)
		{
			if (flag) break;

			std::string sAddress = "$" + hex(nAddr, 4) + ":"; // Address portion
			DrawString(nRamX, nRamY, sAddress, olc::CYAN);    // Draw address in cyan

			std::string sData;
			for (int col = 0; col < nColumns; col++)
			{
				sData += " " + hex(emu_bus.read(nAddr, true), 2); // Data portion
				if (nAddr == maxAddr)
				{
					flag = true;
					break;
				}
				nAddr++;
			}

			DrawString(nRamX + 48, nRamY, sData, olc::WHITE); // Draw data in white (offset slightly)
			nRamY += 10;
		}
	}

	void DrawCpu(int x, int y)
	{
		//std::string status = "STATUS: ";
		DrawString(x , y , "STATUS:", olc::GREY);		
		DrawString(x  + 128, y , "S", emu_bus.cpu.status & cpu8085::S ? olc::GREEN : olc::RED);
		DrawString(x  + 112, y , "Z", emu_bus.cpu.status & cpu8085::Z ? olc::GREEN : olc::RED);
		DrawString(x  + 96, y , "A", emu_bus.cpu.status & cpu8085::A ? olc::GREEN : olc::RED);
		DrawString(x  + 80, y , "P", emu_bus.cpu.status & cpu8085::P ? olc::GREEN : olc::RED);
		DrawString(x  + 64, y, "C", emu_bus.cpu.status & cpu8085::C ? olc::GREEN : olc::RED);
		DrawString(x , y + 10, "PC: $" + hex(emu_bus.cpu.pc, 4),olc::DARK_CYAN);
		DrawString(x , y + 20, "Stack P: $" + hex(emu_bus.cpu.stkp, 4),olc::DARK_GREEN);
		DrawString(x , y + 30, "A: $" +  hex(emu_bus.cpu.a, 2) + "  [" + std::to_string(emu_bus.cpu.a) + "]",olc::MAGENTA);
		DrawString(x , y + 40, "B: $" +  hex(emu_bus.cpu.b, 2) + "  [" + std::to_string(emu_bus.cpu.b) + "]", olc::RED);
		DrawString(x , y + 50, "C: $" +  hex(emu_bus.cpu.c, 2) + "  [" + std::to_string(emu_bus.cpu.c) + "]",olc::CYAN);
		DrawString(x , y + 60, "D: $" +  hex(emu_bus.cpu.d, 2) + "  [" + std::to_string(emu_bus.cpu.d) + "]",olc::BLUE);
		DrawString(x , y + 70, "E: $" +  hex(emu_bus.cpu.e, 2) + "  [" + std::to_string(emu_bus.cpu.e) + "]",olc::YELLOW);
		DrawString(x , y + 80, "H: $" +  hex(emu_bus.cpu.h, 2) + "  [" + std::to_string(emu_bus.cpu.h) + "]",olc::GREY);
		DrawString(x , y + 90, "L: $" +  hex(emu_bus.cpu.l, 2) + "  [" + std::to_string(emu_bus.cpu.l) + "]",olc::GREEN);
	}

	void DrawCode(int x, int y, int nLiemu_bus)
	{
		// Extract dissassembly
		mapAsm = emu_bus.cpu.disassemble(0x0000, 0xFFFF);
		auto it_a = mapAsm.find(emu_bus.cpu.pc);
		int nLineY = (nLiemu_bus >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			DrawString(x, nLineY, (*it_a).second, olc::DARK_CYAN);
			while (nLineY < (nLiemu_bus * 10) + y)
			{
				nLineY += 10;
				if (++it_a != mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second , olc::GREY);
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
					DrawString(x, nLineY, (*it_a).second ,olc::GREY);
				}
			}
		}
	}

	bool OnUserCreate()
	{

		// Writing prog in Ram by transfer content from file
		std::ifstream file(progFilePath); // Open the file
		if (!file.is_open()) {
			std::cerr << "Error: Could not open file="<<progFilePath<< std::endl;
			exit(EXIT_FAILURE);
		}
		
		uint16_t nOffset = progStartAddress; // start address of code
		std::string line;
		while (std::getline(file, line)) { // Read each line
			if (!line.empty()) {
				emu_bus.ram[nOffset] = static_cast<uint8_t>(std::stoul(line, nullptr, 16));
				if(nOffset == 0xFFFF) break;
				nOffset++;
			}
		}

		file.close();
				
		// Extract dissassembly
		mapAsm = emu_bus.cpu.disassemble(0x0000, 0xFFFF);

		// Reset
		emu_bus.cpu.reset(progStartAddress);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		Clear(olc::BLACK);

		if (fullExecutionMode)
		{
			// Run instructions until HALT, NOP, or RST is encountered
			while (!emu_bus.cpu.stop_exe_flag)
			{
				emu_bus.cpu.clock();				
				if (emu_bus.cpu.complete())
					break; // HALT, NOP, or RST encountered
			}
		}

		if (GetKey(olc::Key::SPACE).bPressed) // if space bar key is pressed
		{
			do
			{
				emu_bus.cpu.clock();// actual exe takes place here
			} 
			while (!emu_bus.cpu.complete());
		}

		if (GetKey(olc::Key::R).bPressed)
			emu_bus.cpu.reset(progStartAddress);

	
		//Taking User Input (Address Memory) for viewing Mem Segment
		// Start input mode when 'E' is pressed
		if (GetKey(olc::Key::E).bPressed) {
			inputMode = true;
			userInput.clear();
			firstKeyIgnored = false; // Reset flag
		}

		// Handle text input
		if (inputMode)
		{
			DrawString(10, 405, "Enter Address: $" + userInput + "_", olc::YELLOW);

			for (int k = (int)olc::Key::A; k <= (int)olc::Key::Z; k++)
			{
				if (GetKey((olc::Key)k).bPressed) {
					if (!firstKeyIgnored) {
						firstKeyIgnored = true; // Ignore the first keypress (E)
						continue;
					}
					char keyChar = 'A' + (k - (int)olc::Key::A);
					userInput += keyChar;
				}
			}

			for (int k = (int)olc::Key::K0; k <= (int)olc::Key::K9; k++)
			{
				if (GetKey((olc::Key)k).bPressed) {
					if (!firstKeyIgnored) {
						firstKeyIgnored = true;
						continue;
					}
					char keyChar = '0' + (k - (int)olc::Key::K0);
					userInput += keyChar;
				}
			}

			if (GetKey(olc::Key::BACK).bPressed && !userInput.empty()) {
				userInput.pop_back();
			}

			if (GetKey(olc::Key::ENTER).bPressed && !userInput.empty())
			{
				try {
					userRamAddress = static_cast<uint16_t>(std::stoul(userInput, nullptr, 16));
				} catch (...) {
					userRamAddress = 0x0000;
				}
				inputMode = false;
			}
		}
		/*
		16 col => Each col has 1 byte (8 bit) of data corresponding to 1 address(16 bit)
		==> 1 st row address displayed = 0000h
		==> 2nd row address displayed = 0010h (10h = 16 in dec)
		==> 1st row 1st col data is corresponding to address 0001h
		*/
		DrawString(5,30,"Address Space :- Exam Mem Space(Top)    Prog Space(Bot)",olc::YELLOW);		
		DrawRam(6, 2+y_offset, userRamAddress, 16, 16); // draws start Adress = 0000h and 16 rows and 16 col
		DrawRam(6, 182 +y_offset, progStartAddress, 16, 16); // 1st 2 params (2,182) position on screen where to start draw
		DrawString(463,30,"Registers(Top)     Asm(Bot)",olc::YELLOW);
		DrawCpu(463, 2+y_offset);
		DrawCode(463, 121+y_offset, 26);		
		DrawString(6, 377+y_offset, "SPACE = Step Instruction	R = RESET	E = Exam Mem" ,olc::YELLOW);

		//To make beatiful
		// Draw borders
		DrawRect(4, 0+y_offset, 435, 162, olc::WHITE);  // User Examined RAM
		DrawRect(4, 180+y_offset, 435, 162, olc::WHITE); // Program RAM		
		DrawRect(460, 0+y_offset, 215, 102, olc::WHITE); // Draw CPU Registers		
		DrawRect(460, 118+y_offset, 215, 272, olc::WHITE);// Draw Code Disassembly
		DrawRect(4, 372+y_offset, 435, 17, olc::WHITE);
		// Draw a border around the entire window
		DrawRect(0, 0, ScreenWidth() - 1, ScreenHeight() - 1, olc::WHITE);

		return true;
	}
};



/*					Working
When create pge(main()) --> creates Bus obj(emu_bus) as data member
When emu_bus is created creates cpu obj ( as data member) [init.cpp]
As we press space bar -> exe cpu.clock(); // OnUSerUpdate (exe every frame)
clock() --> executes only 1 instruction
** The code to be executed is Stored in Ram [OnUSerCreate (exe Once)]
*/

int main(int argc, char* argv[])
{
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: " << argv[0] << " <prog start_address> <filename> [--step_wise/--one_go]\n";
        return 1;
    }

	Emulate_cpu8085 pge; //pixel game engine

	pge.progStartAddress = static_cast<uint16_t>(std::stoul(argv[1], nullptr, 16));
	if(!(pge.progStartAddress >= 0x0000 and pge.progStartAddress <= 0xFFFF)){
		std::cerr<< "Invalid program Start Address\n";
		return 1;
	}

	std::string filePath = argv[2];
	if (filePath.ends_with("asm")) {
		Assembler assembler;
		assembler.assemble(filePath, pge.progStartAddress);
		pge.progFilePath = ASSEMBLER_OUTPUT_FILEPATH;
	} else {
		pge.progFilePath = std::string(argv[2]);
	}

    // Determine execution mode
    if (argc == 4) {
        std::string mode = argv[3];
        if (mode == "--one_go")
            pge.fullExecutionMode = true;
        else if (mode != "--step_wise") {
            std::cerr << "Invalid execution mode. Use '--step_wise' or '--one_go'.\n";
            return 1;
        }
    }

	if(pge.Construct(680, 480, 2, 2))
		pge.Start();
	return 0;
}