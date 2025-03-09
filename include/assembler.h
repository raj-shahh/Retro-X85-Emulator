#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <map>

const std::string ASSEMBLER_OUTPUT_FILEPATH = "assembler_output.txt";

struct LineData {
    uint16_t address;
    std::string instruction;
};

class Assembler {
public:
    Assembler();
    std::map<std::string, uint8_t> instructionToOpcode;

    std::vector<uint8_t> assemble(const std::string &filename, uint16_t startAddress);

private:
    static bool is2byteImmediateInstruction(const std::string &mnemonic);

    static bool isIMAInstruction(const std::string &mnemonic);

    static void writeToOpFile(const std::vector<uint8_t> &code, const std::string &filename);

    std::vector<uint8_t> secondPass(const std::vector<LineData> &processedLines,
                                    const std::unordered_map<std::string, uint16_t> &labels);

    static std::pair<std::vector<LineData>, std::unordered_map<std::string, uint16_t> >
    firstPass(const std::vector<std::string> &lines, uint16_t startAddress);

    static std::vector<std::string> readFileLines(const std::string &filename);

    static int parseOperand(const std::string &operand, const std::unordered_map<std::string, uint16_t> &labels);

    static std::string buildOpcodeKey(const std::vector<std::string> &tokens);

    static int getInstructionSize(const std::vector<std::string> &tokens);

    static std::string normalizeInstruction(const std::string &line);

    static std::vector<std::string> tokenize(const std::string &line);

    static std::string trim(const std::string &s);
};
