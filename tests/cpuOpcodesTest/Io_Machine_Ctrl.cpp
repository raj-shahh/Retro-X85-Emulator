#include "cpuTestClass.hpp"

TEST_F(CpuTest, PUSH_B) {
    b = 0x12;
    c = 0x34;
    write(stkp - 1, 0x00);
    write(stkp - 2, 0x00);
    PUSH_B();
    EXPECT_EQ(read(0x0FFE), 0x34);
    EXPECT_EQ(read(0x0FFF), 0x12);
    EXPECT_EQ(stkp, 0x0FFE);
}

TEST_F(CpuTest, POP_B) {
    write(stkp, 0x34);
    write(stkp + 1, 0x12);
    POP_B();
    EXPECT_EQ(b, 0x12);
    EXPECT_EQ(c, 0x34);
    EXPECT_EQ(stkp, 0x1002);
}

TEST_F(CpuTest, XTHL) {
    h = 0x12;
    l = 0x34;
    write(stkp, 0x56);
    write(stkp + 1, 0x78);
    XTHL();
    EXPECT_EQ(h, 0x78);
    EXPECT_EQ(l, 0x56);
    EXPECT_EQ(read(stkp), 0x34);
    EXPECT_EQ(read(stkp + 1), 0x12);
}

TEST_F(CpuTest, SPHL) {
    h = 0x12;
    l = 0x34;
    SPHL();
    EXPECT_EQ(stkp, 0x1234);
}

TEST_F(CpuTest, NOP) {
    stop_exe_flag = false;
    NOP();
    EXPECT_TRUE(stop_exe_flag);
}

TEST_F(CpuTest, HLT) {
    stop_exe_flag = false;
    HLT();
    EXPECT_TRUE(stop_exe_flag);
}

TEST_F(CpuTest, IN) {
    fetched_low = 0x50;
    write(0x5050, 0xAB);
    IN();
    EXPECT_EQ(a, 0xAB);
}

TEST_F(CpuTest, OUT) {
    fetched_low = 0x50;
    a = 0xAB;
    write(0x5050, 0x00);
    OUT();
    EXPECT_EQ(read(0x5050), 0xAB);
}


////////////////////////////// Extra Edge Cases //////////////////////////////

TEST_F(CpuTest, PUSH_B_StackWrapAround) {
    b = 0xAB;
    c = 0xCD;
    stkp = 0x0000; // Edge case: stack wraps around
    PUSH_B();
    EXPECT_EQ(read(0xFFFE), 0xCD);
    EXPECT_EQ(read(0xFFFF), 0xAB);
    EXPECT_EQ(stkp, 0xFFFE);
}

TEST_F(CpuTest, POP_B_StackWrapAround) {
    stkp = 0xFFFE; // Edge case: stack wraps around
    write(0xFFFE, 0xCD);
    write(0xFFFF, 0xAB);
    POP_B();
    EXPECT_EQ(b, 0xAB);
    EXPECT_EQ(c, 0xCD);
    EXPECT_EQ(stkp, 0x0000);
}

TEST_F(CpuTest, XTHL_StackNearEnd) {
    h = 0x01;
    l = 0x02;
    stkp = 0xFFFE; // Edge case: stack near top of memory
    write(0xFFFE, 0x34);
    write(0xFFFF, 0x56);
    XTHL();
    EXPECT_EQ(h, 0x56);
    EXPECT_EQ(l, 0x34);
    EXPECT_EQ(read(0xFFFE), 0x02);
    EXPECT_EQ(read(0xFFFF), 0x01);
}

TEST_F(CpuTest, SPHL_MaxAddress) {
    h = 0xFF;
    l = 0xFF;
    SPHL();
    EXPECT_EQ(stkp, 0xFFFF);
}

TEST_F(CpuTest, NOP_MultipleExecutions) {
    stop_exe_flag = false;
    NOP();
    EXPECT_TRUE(stop_exe_flag);
    stop_exe_flag = false;
    NOP();
    EXPECT_TRUE(stop_exe_flag);
}

TEST_F(CpuTest, HLT_AlreadyHalted) {
    stop_exe_flag = true; // Edge case: CPU already halted
    HLT();
    EXPECT_TRUE(stop_exe_flag);
}

TEST_F(CpuTest, IN_MaxPort) {
    fetched_low = 0xFF; // Edge case: Maximum valid port
    write(0xFFFF, 0x89);
    IN();
    EXPECT_EQ(a, 0x89);
}

TEST_F(CpuTest, OUT_MaxPort) {
    fetched_low = 0xFF; // Edge case: Maximum valid port
    a = 0x76;
    write(0xFFFF, 0x00);
    OUT();
    EXPECT_EQ(read(0xFFFF), 0x76);
}
