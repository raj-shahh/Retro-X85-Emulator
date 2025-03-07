#include "cpuTestClass.hpp"

////////////////////////////// Unconditional Jump //////////////////////////////

TEST_F(CpuTest, JMP) {
    fetched_high = 0x12;
    fetched_low = 0x34;
    JMP();
    EXPECT_EQ(pc, 0x1234);
}

////////////////////////////// Conditional Jumps //////////////////////////////

#define TEST_JUMP_CONDITION(instruction, flag, flag_value, expected_pc) \
    TEST_F(CpuTest, instruction##_##flag_value) { \
        fetched_high = 0x56; \
        fetched_low = 0x78; \
        pc = 0x0000; \
        SetFlag(FLAGS8085::flag, flag_value); \
        instruction(); \
        EXPECT_EQ(pc, expected_pc); \
    }

TEST_JUMP_CONDITION(JNZ, Z, 0, 0x5678)
TEST_JUMP_CONDITION(JNZ, Z, 1, 0x0002)
TEST_JUMP_CONDITION(JZ, Z, 1, 0x5678)
TEST_JUMP_CONDITION(JZ, Z, 0, 0x0002)
TEST_JUMP_CONDITION(JNC, C, 0, 0x5678)
TEST_JUMP_CONDITION(JNC, C, 1, 0x0002)
TEST_JUMP_CONDITION(JC, C, 1, 0x5678)
TEST_JUMP_CONDITION(JC, C, 0, 0x0002)
TEST_JUMP_CONDITION(JPO, P, 0, 0x5678)
TEST_JUMP_CONDITION(JPO, P, 1, 0x0002)
TEST_JUMP_CONDITION(JPE, P, 1, 0x5678)
TEST_JUMP_CONDITION(JPE, P, 0, 0x0002)
TEST_JUMP_CONDITION(JP, S, 0, 0x5678)
TEST_JUMP_CONDITION(JP, S, 1, 0x0002)
TEST_JUMP_CONDITION(JM, S, 1, 0x5678)
TEST_JUMP_CONDITION(JM, S, 0, 0x0002)

////////////////////////////// Unconditional CALL //////////////////////////////

TEST_F(CpuTest, CALL) {
    fetched_high = 0xAB;
    fetched_low = 0xCD;
    pc = 0x5678;
    stkp = 0xFFFE;
    CALL();
    EXPECT_EQ(pc, 0xABCD);
    EXPECT_EQ(read(0xFFFD), 0x56);
    EXPECT_EQ(read(0xFFFC), 0x78);
    EXPECT_EQ(stkp, 0xFFFC);
}

////////////////////////////// Conditional CALL //////////////////////////////

#define TEST_CALL_CONDITION(instruction, flag, flag_value, expected_pc, expected_stkp) \
    TEST_F(CpuTest, instruction##_##flag_value) { \
        fetched_high = 0x22; \
        fetched_low = 0x33; \
        pc = 0x5678; \
        stkp = 0xFFFE; \
        SetFlag(FLAGS8085::flag, flag_value); \
        instruction(); \
        EXPECT_EQ(pc, expected_pc); \
        EXPECT_EQ(stkp, expected_stkp); \
    }

TEST_CALL_CONDITION(CNZ, Z, 0, 0x2233, 0xFFFC)
TEST_CALL_CONDITION(CNZ, Z, 1, 0x567A, 0xFFFE)
TEST_CALL_CONDITION(CZ, Z, 1, 0x2233, 0xFFFC)
TEST_CALL_CONDITION(CZ, Z, 0, 0x567A, 0xFFFE)

////////////////////////////// Unconditional RETURN //////////////////////////////

TEST_F(CpuTest, RET) {
    stkp = 0xFFFC;
    write(0xFFFC, 0x12);
    write(0xFFFD, 0x34);
    RET();
    EXPECT_EQ(pc, 0x3412);
    EXPECT_EQ(stkp, 0xFFFE);
}

////////////////////////////// Conditional RETURN //////////////////////////////

#define TEST_RET_CONDITION(instruction, flag, flag_value, expected_pc, expected_stkp) \
    TEST_F(CpuTest, instruction##_##flag_value) { \
        stkp = 0xFFFC; \
        write(0xFFFC, 0x12); \
        write(0xFFFD, 0x34); \
        SetFlag(FLAGS8085::flag, flag_value); \
        instruction(); \
        EXPECT_EQ(pc, expected_pc); \
        EXPECT_EQ(stkp, expected_stkp); \
    }

TEST_RET_CONDITION(RNZ, Z, 0, 0x3412, 0xFFFE)
TEST_RET_CONDITION(RNZ, Z, 1, 0x0000, 0xFFFC)
TEST_RET_CONDITION(RZ, Z, 1, 0x3412, 0xFFFE)
TEST_RET_CONDITION(RZ, Z, 0, 0x0000, 0xFFFC)