#include "cpuTestClass.hpp"
// ------------------------- INR Tests -------------------------

TEST_F( CpuTest, INR_A) {
     a = 0x3F;
     INR_A();
    EXPECT_EQ( a, 0x40);
}

TEST_F( CpuTest, INR_B) {
     b = 0xFF;
     INR_B();
    EXPECT_EQ( b, 0x00);  // Wraps around
}

TEST_F( CpuTest, INR_M) {
     h = 0x20;
     l = 0x10;
     write(0x2010, 5);  // Default memory value

     INR_M();
    EXPECT_EQ( read(0x2010), 6);
}

// ------------------------- DCR Tests -------------------------

TEST_F( CpuTest, DCR_A) {
     a = 0x3F;
     DCR_A();
    EXPECT_EQ( a, 0x3E);
}

TEST_F( CpuTest, DCR_B) {
     b = 0x00;
     DCR_B();
    EXPECT_EQ( b, 0xFF);  // Underflows
}

TEST_F( CpuTest, DCR_M) {
     h = 0x20;
     l = 0x10;
     write(0x2010, 5);

     DCR_M();
    EXPECT_EQ( read(0x2010), 4);
}

// ------------------------- INX / DCX Tests -------------------------

TEST_F( CpuTest, INX_B) {
     b = 0x12;
     c = 0x34;
     INX_B();
    EXPECT_EQ( b, 0x12);
    EXPECT_EQ( c, 0x35);
}

TEST_F( CpuTest, DCX_B) {
     b = 0x12;
     c = 0x34;
     DCX_B();
    EXPECT_EQ( b, 0x12);
    EXPECT_EQ( c, 0x33);
}

TEST_F( CpuTest, INX_H) {
     h = 0xFF;
     l = 0xFF;
     INX_H();
    EXPECT_EQ( h, 0x00);
    EXPECT_EQ( l, 0x00);
}

TEST_F( CpuTest, DCX_H) {
     h = 0x00;
     l = 0x00;
     DCX_H();
    EXPECT_EQ( h, 0xFF);
    EXPECT_EQ( l, 0xFF);
}

// ------------------------- Stack Pointer Tests -------------------------

TEST_F( CpuTest, INX_SP) {
     stkp = 0x1000;
     INX_SP();
    EXPECT_EQ( stkp, 0x1001);
}

TEST_F( CpuTest, DCX_SP) {
     stkp = 0x1000;
     DCX_SP();
    EXPECT_EQ( stkp, 0x0FFF);
}

// ------------------------- DAA Test -------------------------

TEST_F( CpuTest, DAA) {
     reset(0);
      a = 0x45;
     DAA();
    EXPECT_EQ( a, 0x45);  // No change as no flags are set

    reset(0);
     a = 0x0B;
     SetFlag(FLAGS8085::A, true);
     DAA();
    EXPECT_EQ( a, 0x11);  // Adjusts by 6 due to auxiliary carry

     reset(0);
     a = 0x95;
     SetFlag(FLAGS8085::C, true);
     DAA();
    EXPECT_EQ( a, 0xF5);  // Adds 0x60
}

// ------------------------- Carry Flag Tests -------------------------

TEST_F( CpuTest, CMC) {
     SetFlag(FLAGS8085::C, true);
     CMC();
    EXPECT_FALSE( GetFlag(FLAGS8085::C));

     CMC();
    EXPECT_TRUE( GetFlag(FLAGS8085::C));
}

TEST_F( CpuTest, STC) {
     SetFlag(FLAGS8085::C, false);
     STC();
    EXPECT_TRUE( GetFlag(FLAGS8085::C));
}

// ------------------------- Complement Accumulator Test -------------------------

TEST_F( CpuTest, CMA) {
     a = 0x55;
     CMA();
    EXPECT_EQ( a, 0xAA);
}

// ------------------------- Rotate Instructions -------------------------

TEST_F( CpuTest, RLC) {
     a = 0b10010110;
     RLC();
    EXPECT_EQ( a, 0b00101101);
    EXPECT_TRUE( GetFlag(FLAGS8085::C));  // MSB was 1
}

TEST_F( CpuTest, RRC) {
     a = 0b10010110;
     RRC();
    EXPECT_EQ( a, 0b01001011);
    EXPECT_FALSE( GetFlag(FLAGS8085::C));  // LSB was 0
}

TEST_F( CpuTest, RAL) {
     a = 0b10010110;
     SetFlag(FLAGS8085::C, true);
     RAL();
    EXPECT_EQ( a, 0b00101101);  // Carry shifted in
    EXPECT_TRUE( GetFlag(FLAGS8085::C));  // MSB was 1
}

TEST_F( CpuTest, RAR) {
     a = 0b10010110;
     SetFlag(FLAGS8085::C, true);
     RAR();
    EXPECT_EQ( a, 0b11001011);  // Carry shifted in
    EXPECT_FALSE( GetFlag(FLAGS8085::C));  // LSB was 0
}

// ------------------------- INR Edge Cases -------------------------

// Test INR with Overflow
TEST_F( CpuTest, INR_B_Overflow) {
     b = 0xFF;
     INR_B();
    EXPECT_EQ( b, 0x00);  
    EXPECT_TRUE(GetFlag(FLAGS8085::Z)) << "Zero flag not set after INR overflow";
}

// Test INR causing Zero Flag
TEST_F( CpuTest, INR_A_ZeroFlag) {
     a = 0xFF;
     INR_A();
    EXPECT_EQ( a, 0x00);
    EXPECT_TRUE(GetFlag(FLAGS8085::Z)) << "Zero flag not set after INR caused 0";
}

// ------------------------- DCR Edge Cases -------------------------

// Test DCR with Underflow (Negative Result)
TEST_F( CpuTest, DCR_B_Underflow) {
     b = 0x00;
     DCR_B();
    EXPECT_EQ( b, 0xFF);
    EXPECT_TRUE(GetFlag(FLAGS8085::S)) << "Sign flag not set on underflow";
}

// Test DCR with Zero Result
TEST_F( CpuTest, DCR_A_ZeroFlag) {
     a = 0x01;
     DCR_A();
    EXPECT_EQ( a, 0x00);
    EXPECT_TRUE(GetFlag(FLAGS8085::Z)) << "Zero flag not set";
}

// ------------------------- INX / DCX Edge Cases -------------------------

// Test INX_B causing Carry to High Byte
TEST_F( CpuTest, INX_B_CarryToHigh) {
     b = 0x12;
     c = 0xFF;
     INX_B();
    EXPECT_EQ( b, 0x13);
    EXPECT_EQ( c, 0x00);
}

// Test DCX_B causing Borrow from High Byte
TEST_F( CpuTest, DCX_B_BorrowFromHigh) {
     b = 0x12;
     c = 0x00;
     DCX_B();
    EXPECT_EQ( b, 0x11);
    EXPECT_EQ( c, 0xFF);
}

// ------------------------- Stack Pointer Edge Cases -------------------------

// Test INX_SP at Upper Bound
TEST_F( CpuTest, INX_SP_Overflow) {
     stkp = 0xFFFF;
     INX_SP();
    EXPECT_EQ( stkp, 0x0000);
}

// Test DCX_SP at Lower Bound
TEST_F( CpuTest, DCX_SP_Underflow) {
     stkp = 0x0000;
     DCX_SP();
    EXPECT_EQ( stkp, 0xFFFF);
}

// ------------------------- DAA Edge Cases -------------------------

// Test DAA with Half-Carry Flag
TEST_F( CpuTest, DAA_HalfCarry) {
     a = 0x09;
     SetFlag(FLAGS8085::A, true);
     DAA();
    EXPECT_EQ( a, 0x0F);
}

// Test DAA with Carry Flag
TEST_F( CpuTest, DAA_Carry) {
     a = 0x99;
     SetFlag(FLAGS8085::C, true);
     DAA();
    EXPECT_EQ( a, 0xF9);
}

// ------------------------- Carry Flag Edge Cases -------------------------

// Test CMC toggling multiple times
TEST_F( CpuTest, CMC_Toggle) {
     SetFlag(FLAGS8085::C, true);
     CMC();
    EXPECT_FALSE(GetFlag(FLAGS8085::C));

     CMC();
    EXPECT_TRUE(GetFlag(FLAGS8085::C));

     CMC();
    EXPECT_FALSE(GetFlag(FLAGS8085::C));
}

// ------------------------- CMA Edge Cases -------------------------

// Test CMA on Special Values
TEST_F( CpuTest, CMA_SpecialValues) {
     a = 0x00;
     CMA();
    EXPECT_EQ( a, 0xFF);

     a = 0xFF;
     CMA();
    EXPECT_EQ( a, 0x00);
}

// ------------------------- Rotate Instructions Edge Cases -------------------------

// Test RLC where MSB is 0
TEST_F( CpuTest, RLC_NoCarry) {
     a = 0b01100000;
     RLC();
    EXPECT_EQ( a, 0b11000000);
    EXPECT_FALSE(GetFlag(FLAGS8085::C));  // No carry since MSB was 0
}

// Test RRC where LSB is 1
TEST_F( CpuTest, RRC_WithCarry) {
     a = 0b00000001;
     RRC();
    EXPECT_EQ( a, 0b10000000);
    EXPECT_TRUE(GetFlag(FLAGS8085::C));  // LSB was 1, so carry set
}

// Test RAL where Carry Flag is 0
TEST_F( CpuTest, RAL_NoInitialCarry) {
     a = 0b01010101;
     SetFlag(FLAGS8085::C, false);
     RAL();
    EXPECT_EQ( a, 0b10101010);
    EXPECT_FALSE(GetFlag(FLAGS8085::C));  // No carry before, MSB is not 1
}

// Test RAR where Carry Flag is 1
TEST_F( CpuTest, RAR_WithInitialCarry) {
     a = 0b00000000;
     SetFlag(FLAGS8085::C, true);
     RAR();
    EXPECT_EQ( a, 0b10000000);
    EXPECT_FALSE(GetFlag(FLAGS8085::C));  // LSB was 0, so carry cleared
}
