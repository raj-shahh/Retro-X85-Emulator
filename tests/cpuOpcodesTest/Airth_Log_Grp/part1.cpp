#include "cpuTestClass.hpp"


// Test Add with Register
TEST_F(CpuTest, AddRegisters_ADD_C) {
    a = 10;
    c = 6;
    ADD_C();
    ASSERT_EQ(a, 16) << "ADD_C() failed";
}

// Test ADD with Memory
TEST_F(CpuTest, AddRegisters_ADD_M) {
    a = 4;
    h = 0x20; l = 0x10; // HL points to 0x2010
    write(0x2010,0x05);
    ADD_M();
    ASSERT_EQ(a, 9) << "ADD_M() failed";
}

// Test ADI (Immediate Addition)
TEST_F(CpuTest, AddImmediate_ADI) {
    a = 7;
    fetched_low = 8;
    ADI();
    ASSERT_EQ(a, 15) << "ADI() failed";
}

// Test ADC with carry
TEST_F(CpuTest, AddWithCarry_ADC_B) {
    a = 5;
    b = 2;
    SetFlag(FLAGS8085::C, true);
    ADC_B();
    ASSERT_EQ(a, 8) << "ADC_B() failed"; // 5 + 2 + 1 (carry)
}

// Test SUB instructions
TEST_F(CpuTest, SubtractRegisters_SUB_B) {
    a = 10;
    b = 3;
    SUB_B();
    ASSERT_EQ(a, 7) << "SUB_B() failed";
}

// Test SBB (Subtract with Borrow)
TEST_F(CpuTest, SubtractWithBorrow_SBB_C) {
    a = 10;
    c = 4;
    SetFlag(FLAGS8085::C, true);
    SBB_C();
    ASSERT_EQ(a, 5) << "SBB_C() failed"; // 10 - (4 + 1)
}

// Test DAD (Register pair addition)
TEST_F(CpuTest, DoubleAdd_DAD_B) {
    h = 0x12; l = 0x34;
    b = 0x01; c = 0x10;
    DAD_B();
    ASSERT_EQ(h, 0x13) << "DAD_B() HL high byte mismatch";
    ASSERT_EQ(l, 0x44) << "DAD_B() HL low byte mismatch";
}

// Test DAD with Stack Pointer
TEST_F(CpuTest, DoubleAdd_DAD_SP) {
    h = 0x12; l = 0x34;
    stkp = 0x0100;
    DAD_SP();
    ASSERT_EQ(h, 0x13) << "DAD_SP() HL high byte mismatch";
    ASSERT_EQ(l, 0x34) << "DAD_SP() HL low byte mismatch";
}

