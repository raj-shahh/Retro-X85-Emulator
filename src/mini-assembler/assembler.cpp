#include "assembler.h"

bool Assembler::is2byteImmediateInstruction(const std::string &mnemonic) {
    return mnemonic == "ADI" || mnemonic == "SUI" || mnemonic == "ANI" || mnemonic == "ORI" ||
           mnemonic == "XRI" || mnemonic == "CPI" || mnemonic == "ACI" || mnemonic == "SBI" || mnemonic == "IN" ||
           mnemonic == "OUT";
}

bool Assembler::isIMAInstruction(const std::string &mnemonic) {
    if (mnemonic == "SHLD" || mnemonic == "LHLD" || mnemonic == "STA" || mnemonic == "LDA")
        return true;

    if (mnemonic[0] == 'J')
        return true;

    if (mnemonic[0] == 'C' && mnemonic != "CMA" && !mnemonic.starts_with("CMP") && mnemonic != "CMC")
        return true;

    return false;
}

std::string Assembler::trim(const std::string &s) {
    size_t start = s.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
}


std::string Assembler::normalizeInstruction(const std::string &instr) {
    std::string trimmed = trim(instr);
    std::string result;
    for (size_t i = 0; i < trimmed.size(); i++) {
        char c = trimmed[i];
        if (c == ',') {
            result.push_back(',');
            while (i + 1 < trimmed.size() && std::isspace(trimmed[i + 1])) {
                i++;
            }
        } else if (std::isspace(c)) {
            if (result.empty() || result.back() != ' ')
                result.push_back(' ');
        } else {
            result.push_back(std::toupper(c));
        }
    }
    return result;
}

std::vector<std::string> Assembler::tokenize(const std::string &line) {
    std::vector<std::string> tokens;
    char *cstr = new char[line.length() + 1];
    std::strcpy(cstr, line.c_str());

    char *token = std::strtok(cstr, " ,");
    while (token != nullptr) {
        tokens.push_back(token);
        token = std::strtok(nullptr, " ,");
    }

    delete[] cstr;
    return tokens;
}

int Assembler::getInstructionSize(const std::vector<std::string> &tokens) {
    if (tokens.empty())
        return 0;
    const std::string &mnemonic = tokens[0];

    if (mnemonic == "MVI" || is2byteImmediateInstruction(mnemonic)) {
        return 2;
    }

    if (mnemonic == "LXI" || isIMAInstruction(mnemonic)) return 3;

    return 1;
}

std::string Assembler::buildOpcodeKey(const std::vector<std::string> &tokens) {
    if (tokens.empty())
        return "";
    std::string mnemonic = tokens[0];

    if (mnemonic == "MVI" || mnemonic == "LXI") {
        if (tokens.size() < 2)
            throw std::runtime_error("Missing register operand for immediate instruction.");
        if (tokens.size() < 3)
            throw std::runtime_error("Missing address operand for immediate instruction.");
        return mnemonic + " " + tokens[1];
    }

    if (isIMAInstruction(mnemonic)) {
        if (tokens.size() < 2)
            throw std::runtime_error("Missing address operand for immediate address instruction.");
        return mnemonic;
    }

    if (mnemonic == "ADI" || mnemonic == "SUI" || mnemonic == "ANI" || mnemonic == "ORI" ||
        mnemonic == "XRI" || mnemonic == "CPI" || mnemonic == "ACI" || mnemonic == "SBI" || mnemonic == "IN" ||
        mnemonic == "OUT") {
        if (tokens.size() < 2)
            throw std::runtime_error("Missing data operand for immediate address instruction.");
        return mnemonic;
    }

    if (tokens.size() == 3)
        return tokens[0] + " " + tokens[1] + "," + tokens[2];

    if (tokens.size() == 2)
        return tokens[0] + " " + tokens[1];

    return tokens[0];
}

int Assembler::parseOperand(const std::string &operand, const std::unordered_map<std::string, uint16_t> &labels) {
    try {
        return std::stoi(operand, nullptr, 0);
    } catch (...) {
        auto it = labels.find(operand);
        if (it != labels.end())
            return it->second;
        throw std::runtime_error("Unknown operand: " + operand);
    }
}

std::vector<std::string> Assembler::readFileLines(const std::string &filename) {
    std::vector<std::string> lines;
    std::ifstream fin(filename);
    if (!fin)
        throw std::runtime_error("Error opening file: " + filename);
    std::string line;
    while (std::getline(fin, line))
        lines.push_back(line);
    return lines;
}


std::pair<std::vector<LineData>, std::unordered_map<std::string, uint16_t> >
Assembler::firstPass(const std::vector<std::string> &lines, uint16_t startAddress) {
    std::unordered_map<std::string, uint16_t> labels;
    std::vector<LineData> processedLines;
    uint16_t currentAddress = startAddress;

    for (const auto &orig_line: lines) {
        std::string line = orig_line;

        // Remove comments (anything after ';')
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        line = trim(line);
        if (line.empty())
            continue;

        // Check for a label (assumes label ends with ':')
        size_t colonPos = line.find(':');
        while (colonPos != std::string::npos) {
            std::string label = trim(line.substr(0, colonPos));
            labels[label] = currentAddress;

            // Remove label from line (if there are multiple labels, loop again)
            line = trim(line.substr(colonPos + 1));
            colonPos = line.find(':');
        }

        if (line.empty())
            continue;


        std::string normInstr = normalizeInstruction(line);

        std::vector<std::string> tokens = tokenize(normInstr);
        int size = getInstructionSize(tokens);
        processedLines.push_back({currentAddress, normInstr});
        currentAddress += size;
    }
    return {processedLines, labels};
}


std::vector<uint8_t> Assembler::secondPass(const std::vector<LineData> &processedLines,
                                const std::unordered_map<std::string, uint16_t> &labels) {
    std::vector<uint8_t> machineCode;
    for (const auto &ld: processedLines) {
        std::vector<std::string> tokens = tokenize(ld.instruction);
        if (tokens.empty())
            continue;

        std::string key = buildOpcodeKey(tokens);
        if (!instructionToOpcode.contains(key))
            throw std::runtime_error("Opcode not found for instruction: " + key);

        const int instructionLength = getInstructionSize(tokens);
        uint8_t opcode = instructionToOpcode[key];
        machineCode.push_back(opcode);
        if (instructionLength == 2) {
            machineCode.push_back(parseOperand(tokens[std::size(tokens) - 1], labels));
        } else if (instructionLength == 3) {
            uint16_t addr = parseOperand(tokens[std::size(tokens) - 1], labels);
            machineCode.push_back(addr & 0xFF);
            machineCode.push_back((addr >> 8) & 0xFF);
        }
    }
    return machineCode;
}

void Assembler::writeToOpFile(const std::vector<uint8_t> &code, const std::string &filename) {
    FILE *fout = fopen(filename.c_str(), "w");
    if (!fout)
        throw std::runtime_error("Error opening file: " + filename);

    for (int i = 0; i < code.size(); i++) {
        fprintf(fout, "0x%02X\n", code[i]);
    }
    fclose(fout);
}

std::vector<uint8_t> Assembler::assemble(const std::string &filepath, uint16_t startAddress) {
    std::vector<std::string> lines = readFileLines(filepath);

    auto firstPassResult = firstPass(lines, startAddress);
    std::vector<LineData> processedLines = firstPassResult.first;
    std::unordered_map<std::string, uint16_t> labelMap = firstPassResult.second;
    std::vector<uint8_t> code = secondPass(processedLines, labelMap);

    writeToOpFile(code, ASSEMBLER_OUTPUT_FILEPATH);
    return code;
}
