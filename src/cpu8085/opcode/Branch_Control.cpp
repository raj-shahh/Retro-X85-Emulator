#include "cpu8085.h"
#include "Bus.h"

#define ADDITIONAL_CONDITIONAL_CALL_CYCLES 3
#define ADDITIONAL_CONDITIONAL_RET_CYCLES 2
#define ADDITIONAL_CONDITIONAL_JUMP_CYCLES 1


uint8_t cpu8085::JMP() {
    pc = addr_abs;
    return 0;
}

uint8_t cpu8085::JNZ() {
    if (GetFlag(FLAGS8085::Z) == 0) {
        JMP();
        return ADDITIONAL_CONDITIONAL_JUMP_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::JZ() {
    if (GetFlag(FLAGS8085::Z) == 1) {
        JMP();
        return ADDITIONAL_CONDITIONAL_JUMP_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::JNC() {
    if (GetFlag(FLAGS8085::C) == 0) {
        JMP();
        return ADDITIONAL_CONDITIONAL_JUMP_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::JC() {
    if (GetFlag(FLAGS8085::C) == 1) {
        JMP();
        return ADDITIONAL_CONDITIONAL_JUMP_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::JPO() {
    if (GetFlag(FLAGS8085::P) == 0) {
        JMP();
        return ADDITIONAL_CONDITIONAL_JUMP_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::JPE() {
    if (GetFlag(FLAGS8085::P) == 1) {
        JMP();
        return ADDITIONAL_CONDITIONAL_JUMP_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::JP() {
    if (GetFlag(FLAGS8085::S) == 0) {
        JMP();
        return ADDITIONAL_CONDITIONAL_JUMP_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::JM() {
    if (GetFlag(FLAGS8085::S) == 1) {
        JMP();
        return ADDITIONAL_CONDITIONAL_JUMP_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::PCHL() {
    pc = (static_cast<uint16_t>(h) << 8) | static_cast<uint16_t>(l);
    return 0;
}

uint8_t cpu8085::CALL() {
    write(stkp - 1, (pc >> 8) & 0xFF);
    write(stkp - 2, pc & 0xFF);
    stkp -= 2;

    pc = addr_abs;
    return 0;
}

uint8_t cpu8085::CNZ() {
    if (GetFlag(FLAGS8085::Z) == 0) {
        CALL();
        return ADDITIONAL_CONDITIONAL_CALL_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::CZ() {
    if (GetFlag(FLAGS8085::Z) == 1) {
        CALL();
        return ADDITIONAL_CONDITIONAL_CALL_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::CNC() {
    if (GetFlag(FLAGS8085::C) == 0) {
        CALL();
        return ADDITIONAL_CONDITIONAL_CALL_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::CC() {
    if (GetFlag(FLAGS8085::C) == 1) {
        CALL();
        return ADDITIONAL_CONDITIONAL_CALL_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::CPO() {
    if (GetFlag(FLAGS8085::P) == 0) {
        CALL();
        return ADDITIONAL_CONDITIONAL_CALL_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::CPE() {
    if (GetFlag(FLAGS8085::P) == 1) {
        CALL();
        return ADDITIONAL_CONDITIONAL_CALL_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::CP() {
    if (GetFlag(FLAGS8085::S) == 0) {
        CALL();
        return ADDITIONAL_CONDITIONAL_CALL_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::CM() {
    if (GetFlag(FLAGS8085::S) == 1) {
        CALL();
        return ADDITIONAL_CONDITIONAL_CALL_CYCLES;
    }
    pc += 2;
    return 0;
}

uint8_t cpu8085::RET() {
    pc = read(stkp) | (read(stkp + 1) << 8);
    stkp += 2;
    return 0;
}

uint8_t cpu8085::RNZ() {
    if (GetFlag(FLAGS8085::Z) == 0) {
        RET();
        return ADDITIONAL_CONDITIONAL_RET_CYCLES;
    }
    return 0;
}

uint8_t cpu8085::RZ() {
    if (GetFlag(FLAGS8085::Z) == 1) {
        RET();
        return ADDITIONAL_CONDITIONAL_RET_CYCLES;
    }
    return 0;
}

uint8_t cpu8085::RNC() {
    if (GetFlag(FLAGS8085::C) == 0) {
        RET();
        return ADDITIONAL_CONDITIONAL_RET_CYCLES;
    }
    return 0;
}

uint8_t cpu8085::RC() {
    if (GetFlag(FLAGS8085::C) == 1) {
        RET();
        return ADDITIONAL_CONDITIONAL_RET_CYCLES;
    }
    return 0;
}

uint8_t cpu8085::RPO() {
    if (GetFlag(FLAGS8085::P) == 0) {
        RET();
        return ADDITIONAL_CONDITIONAL_RET_CYCLES;
    }
    return 0;
}

uint8_t cpu8085::RPE() {
    if (GetFlag(FLAGS8085::P) == 1) {
        RET();
        return ADDITIONAL_CONDITIONAL_RET_CYCLES;
    }
    return 0;
}

uint8_t cpu8085::RP() {
    if (GetFlag(FLAGS8085::S) == 0) {
        RET();
        return ADDITIONAL_CONDITIONAL_RET_CYCLES;
    }
    return 0;
}

uint8_t cpu8085::RM() {
    if (GetFlag(FLAGS8085::S) == 1) {
        RET();
        return ADDITIONAL_CONDITIONAL_RET_CYCLES;
    }
    return 0;
}
