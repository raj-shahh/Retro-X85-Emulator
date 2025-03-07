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

////////////////////////////// Edge Cases //////////////////////

#include "cpuTestClass.hpp"

// Test ADD with Overflow
TEST_F(CpuTest, AddRegisters_ADD_C_Overflow) {
    a = 250;
    c = 10;
    ADD_C();
    ASSERT_EQ(a, 4) << "ADD_C() Overflow failed"; // 250 + 10 = 260 (0x104), lower byte remains (wraps)
    ASSERT_TRUE(GetFlag(FLAGS8085::C)) << "Carry flag not set on overflow";
}

// Test ADD with Zero Result
TEST_F(CpuTest, AddRegisters_ADD_C_Zero) {
    a = 0;
    c = 0;
    ADD_C();
    ASSERT_EQ(a, 0) << "ADD_C() Zero case failed";
    ASSERT_TRUE(GetFlag(FLAGS8085::Z)) << "Zero flag not set";
}

// Test ADD Memory with Overflow
TEST_F(CpuTest, AddRegisters_ADD_M_Overflow) {
    a = 0xF0;
    h = 0x20; l = 0x10;
    write(0x2010, 0x20);
    ADD_M();
    ASSERT_EQ(a, 0x10) << "ADD_M() Overflow failed"; // 0xF0 + 0x20 = 0x110
    ASSERT_TRUE(GetFlag(FLAGS8085::C)) << "Carry flag not set";
}

// Test ADI with Overflow
TEST_F(CpuTest, AddImmediate_ADI_Overflow) {
    a = 0xF8;
    fetched_low = 0x10;
    ADI();
    ASSERT_EQ(a, 0x08) << "ADI() Overflow failed"; // 0xF8 + 0x10 = 0x108
    ASSERT_TRUE(GetFlag(FLAGS8085::C)) << "Carry flag not set";
}

// Test ADC with No Carry
TEST_F(CpuTest, AddWithCarry_ADC_B_NoCarry) {
    a = 5;
    b = 2;
    SetFlag(FLAGS8085::C, false);
    ADC_B();
    ASSERT_EQ(a, 7) << "ADC_B() failed"; // 5 + 2 + 0
}

// Test SUB with Resulting Zero
TEST_F(CpuTest, SubtractRegisters_SUB_B_Zero) {
    a = 5;
    b = 5;
    SUB_B();
    ASSERT_EQ(a, 0) << "SUB_B() Zero case failed";
    ASSERT_TRUE(GetFlag(FLAGS8085::Z)) << "Zero flag not set";
}

// Test SUB with Underflow (Negative Result)
TEST_F(CpuTest, SubtractRegisters_SUB_B_Negative) {
    a = 3;
    b = 5;
    SUB_B();
    ASSERT_EQ(a, 0xFE) << "SUB_B() Underflow failed"; // 3 - 5 = -2 (0xFE in unsigned 8-bit)
    ASSERT_TRUE(GetFlag(FLAGS8085::S)) << "Sign flag not set"; // Negative result sets Sign flag
}

// Test SBB with No Borrow
TEST_F(CpuTest, SubtractWithBorrow_SBB_C_NoBorrow) {
    a = 10;
    c = 4;
    SetFlag(FLAGS8085::C, false);
    SBB_C();
    ASSERT_EQ(a, 6) << "SBB_C() No Borrow failed"; // 10 - 4 - 0
}

// Test SBB with Borrow
TEST_F(CpuTest, SubtractWithBorrow_SBB_C_WithBorrow) {
    a = 10;
    c = 4;
    SetFlag(FLAGS8085::C, true);
    SBB_C();
    ASSERT_EQ(a, 5) << "SBB_C() With Borrow failed"; // 10 - (4 + 1)
}

// Test DAD with Overflow (Carry from HL)
TEST_F(CpuTest, DoubleAdd_DAD_B_Overflow) {
    h = 0xFF; l = 0xFF;
    b = 0x00; c = 0x02;
    DAD_B();
    ASSERT_EQ(h, 0x00) << "DAD_B() High byte overflow failed";
    ASSERT_EQ(l, 0x01) << "DAD_B() Low byte overflow failed"; // 0xFFFF + 0x0002 = 0x0001 (carry)
    ASSERT_TRUE(GetFlag(FLAGS8085::C)) << "Carry flag not set on overflow";
}

// Test DAD with Stack Pointer and Carry
TEST_F(CpuTest, DoubleAdd_DAD_SP_Carry) {
    h = 0x80; l = 0x00;
    stkp = 0x8000;
    DAD_SP();
    ASSERT_EQ(h, 0x00) << "DAD_SP() Carry high byte failed";
    ASSERT_EQ(l, 0x00) << "DAD_SP() Carry low byte failed"; // 0x8000 + 0x8000 = 0x0000 (carry)
    ASSERT_TRUE(GetFlag(FLAGS8085::C)) << "Carry flag not set";
}


