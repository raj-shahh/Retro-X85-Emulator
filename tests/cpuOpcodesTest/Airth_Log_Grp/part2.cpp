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
     a = 0x45;
     DAA();
    EXPECT_EQ( a, 0x45);  // No change as no flags are set

     a = 0x0B;
     SetFlag(FLAGS8085::A, true);
     DAA();
    EXPECT_EQ( a, 0x11);  // Adjusts by 6 due to auxiliary carry

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
