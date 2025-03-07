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


////////////////////////////// Extra Edge Cases //////////////////////////////

TEST_F(CpuTest, JMP_MaxAddress) {
    fetched_high = 0xFF;
    fetched_low = 0xFF;
    JMP();
    EXPECT_EQ(pc, 0xFFFF);
}

TEST_F(CpuTest, CALL_StackWrapAround) {
    fetched_high = 0x12;
    fetched_low = 0x34;
    pc = 0x5678;
    stkp = 0x0000; // Edge case: stack wraps around
    CALL();
    EXPECT_EQ(pc, 0x1234);
    EXPECT_EQ(read(0xFFFF), 0x56);
    EXPECT_EQ(read(0xFFFE), 0x78);
    EXPECT_EQ(stkp, 0xFFFE);
}

TEST_F(CpuTest, RET_StackWrapAround) {
    stkp = 0xFFFF;
    write(0xFFFF, 0xAB);
    write(0x0000, 0xCD); // Edge case: stack wraps around
    RET();
    EXPECT_EQ(pc, 0xCDAB);
    EXPECT_EQ(stkp, 0x0001);
}

TEST_F(CpuTest, CALL_NearEndMemory) {
    fetched_high = 0xFE;
    fetched_low = 0xFF;
    pc = 0xFFFF;
    stkp = 0xFFFE;
    CALL();
    EXPECT_EQ(pc, 0xFEFF);
    EXPECT_EQ(read(0xFFFD), 0xFF);
    EXPECT_EQ(read(0xFFFC), 0xFF);
    EXPECT_EQ(stkp, 0xFFFC);
}

TEST_F(CpuTest, RET_NearEndMemory) {
    stkp = 0xFFFE;
    write(0xFFFE, 0x00);
    write(0xFFFF, 0x80); // Edge case: returning to high address
    RET();
    EXPECT_EQ(pc, 0x8000);
    EXPECT_EQ(stkp, 0x0000);
}

TEST_F(CpuTest, JNZ_MaxPC_NoJump) {
    fetched_high = 0x80;
    fetched_low = 0x00;
    pc = 0xFFFF; // Edge case: Max PC value, should not jump
    SetFlag(FLAGS8085::Z, 1);
    JNZ();
    EXPECT_EQ(pc, 0x0001);
}

TEST_F(CpuTest, JZ_MaxPC_Jump) {
    fetched_high = 0x80;
    fetched_low = 0x00;
    pc = 0xFFFF; // Edge case: Max PC value, should jump
    SetFlag(FLAGS8085::Z, 1);
    JZ();
    EXPECT_EQ(pc, 0x8000);
}

TEST_F(CpuTest, CNZ_MinStack) {
    fetched_high = 0x44;
    fetched_low = 0x55;
    pc = 0x1234;
    stkp = 0x0002; // Edge case: Stack near bottom
    SetFlag(FLAGS8085::Z, 0);
    CNZ();
    EXPECT_EQ(pc, 0x4455);
    EXPECT_EQ(read(0x0001), 0x12);
    EXPECT_EQ(read(0x0000), 0x34);
    EXPECT_EQ(stkp, 0x0000);
}

TEST_F(CpuTest, RNZ_MinStack) {
    stkp = 0x0000; // Edge case: Stack at bottom
    write(0x0000, 0x34);
    write(0x0001, 0x12);
    SetFlag(FLAGS8085::Z, 0);
    RNZ();
    EXPECT_EQ(pc, 0x1234);
    EXPECT_EQ(stkp, 0x0002);
}

