
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
	uint16_t progStartAddress;
	std::string progFilePath;

	bool inputMode = false;          // Flag to check if input mode is active
	std::string userInput = "";      // Stores user-entered address
	uint16_t userRamAddress = 0x0000; // Default RAM display address
	bool firstKeyIgnored = false; // NEW: Track first keypress

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
			if(flag) break;
			std::string sOffset = "$" + hex(nAddr, 4) + ":";		
			for (int col = 0; col < nColumns; col++)
			{
				sOffset += " " + hex(emu_bus.read(nAddr, true), 2);
				if(nAddr== maxAddr){
					flag = true;
					break;
				}
				nAddr++;
			}

			DrawString(nRamX, nRamY, sOffset);
			nRamY += 10;
		}
	}


	void DrawCpu(int x, int y)
	{
		//std::string status = "STATUS: ";
		DrawString(x , y , "STATUS:", olc::WHITE);
		DrawString(x  + 64, y, "C", emu_bus.cpu.status & cpu8085::C ? olc::GREEN : olc::RED);
		DrawString(x  + 80, y , "P", emu_bus.cpu.status & cpu8085::P ? olc::GREEN : olc::RED);
		DrawString(x  + 96, y , "A", emu_bus.cpu.status & cpu8085::A ? olc::GREEN : olc::RED);
		DrawString(x  + 112, y , "Z", emu_bus.cpu.status & cpu8085::Z ? olc::GREEN : olc::RED);
		DrawString(x  + 128, y , "S", emu_bus.cpu.status & cpu8085::S ? olc::GREEN : olc::RED);
		DrawString(x , y + 10, "PC: $" + hex(emu_bus.cpu.pc, 4));
		DrawString(x , y + 20, "Stack P: $" + hex(emu_bus.cpu.stkp, 4));
		DrawString(x , y + 30, "A: $" +  hex(emu_bus.cpu.a, 2) + "  [" + std::to_string(emu_bus.cpu.a) + "]");
		DrawString(x , y + 40, "B: $" +  hex(emu_bus.cpu.b, 2) + "  [" + std::to_string(emu_bus.cpu.b) + "]");
		DrawString(x , y + 50, "C: $" +  hex(emu_bus.cpu.c, 2) + "  [" + std::to_string(emu_bus.cpu.c) + "]");
		DrawString(x , y + 60, "D: $" +  hex(emu_bus.cpu.d, 2) + "  [" + std::to_string(emu_bus.cpu.d) + "]");
		DrawString(x , y + 70, "E: $" +  hex(emu_bus.cpu.e, 2) + "  [" + std::to_string(emu_bus.cpu.e) + "]");
		DrawString(x , y + 80, "H: $" +  hex(emu_bus.cpu.h, 2) + "  [" + std::to_string(emu_bus.cpu.h) + "]");
		DrawString(x , y + 90, "L: $" +  hex(emu_bus.cpu.l, 2) + "  [" + std::to_string(emu_bus.cpu.l) + "]");
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
				emu_bus.ram[nOffset++] = static_cast<uint8_t>(std::stoul(line, nullptr, 16));
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


		if (GetKey(olc::Key::SPACE).bPressed) // if space bar key is pressed
		{
			do
			{
				emu_bus.cpu.clock();
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
        DrawString(10, 400, "Enter Address: $" + userInput + "_", olc::YELLOW);

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
		DrawRam(2, 2, userRamAddress, 16, 16); // draws start Adress = 0000h and 16 rows and 16 col
		DrawRam(2, 182, progStartAddress, 16, 16); // 1st 2 params (2,182) position on screen where to start draw
		DrawCpu(448, 2);
		DrawCode(448, 121, 26);		
		DrawString(2, 370, "SPACE = Step Instruction	R = RESET	E = Exam Mem");

		//To make beatiful
		// Draw borders
		DrawRect(0, 0, 435, 162, olc::WHITE);  // User Examined RAM
		DrawRect(0, 180, 435, 162, olc::WHITE); // Program RAM		
		DrawRect(445, 0, 230, 102, olc::WHITE); // Draw CPU Registers		
		DrawRect(445, 118, 230, 272, olc::WHITE);// Draw Code Disassembly
		DrawRect(0, 365, 435, 17, olc::WHITE);

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

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Emu Usage: " << argv[0] << " <prog start_address> <filename>" << std::endl;
        return 1;
    }

	Emulate_cpu8085 pge; //pixel game engine

	pge.progFilePath = std::string(argv[2]);
	pge.progStartAddress = static_cast<uint16_t>(std::stoul(argv[1], nullptr, 16)); 

	if(!(pge.progStartAddress >= 0x0000 and pge.progStartAddress <= 0xFFFF)){
		std::cerr<< "Invalid program Start Address\n";
		return 1;
	}

	if(pge.Construct(680, 480, 2, 2))
		pge.Start();
	return 0;
}