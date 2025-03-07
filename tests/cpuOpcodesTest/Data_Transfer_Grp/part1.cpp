#include "cpuTestClass.hpp"

TEST_F(CpuTest, MOV_A_B) {
    b = 0x55;
    MOV_A_B();
    EXPECT_EQ(a, 0x55);
}

TEST_F(CpuTest, MOV_A_Memory) {
    h = 0x20;
    l = 0x10;
    write(0x2010, 0xAA);
    MOV_A_M();
    EXPECT_EQ(a, 0xAA);
}

TEST_F(CpuTest, MOV_B_A) {
    a = 0x33;
    MOV_B_A();
    EXPECT_EQ(b, 0x33);
}

TEST_F(CpuTest, MOV_B_Memory) {
    h = 0x30;
    l = 0x20;
    write(0x3020, 0x77);
    MOV_B_M();
    EXPECT_EQ(b, 0x77);
}

TEST_F(CpuTest, MOV_C_D) {
    d = 0x99;
    MOV_C_D();
    EXPECT_EQ(c, 0x99);
}

TEST_F(CpuTest, MOV_C_Memory) {
    h = 0x40;
    l = 0x30;
    write(0x4030, 0x88);
    MOV_C_M();
    EXPECT_EQ(c, 0x88);
}

TEST_F(CpuTest, MOV_D_E) {
    e = 0x11;
    MOV_D_E();
    EXPECT_EQ(d, 0x11);
}

TEST_F(CpuTest, MOV_D_Memory) {
    h = 0x50;
    l = 0x40;
    write(0x5040, 0x66);
    MOV_D_M();
    EXPECT_EQ(d, 0x66);
}


// ------------------------- MOV Edge Cases -------------------------

// Test MOV A, A (Self Assignment)
TEST_F(CpuTest, MOV_A_A) {
    a = 0x77;
    MOV_A_A();
    EXPECT_EQ(a, 0x77);
}

// Test MOV B, B (Self Assignment)
TEST_F(CpuTest, MOV_B_B) {
    b = 0x55;
    MOV_B_B();
    EXPECT_EQ(b, 0x55);
}

// Test MOV A, B with 0x00
TEST_F(CpuTest, MOV_A_B_Zero) {
    b = 0x00;
    MOV_A_B();
    EXPECT_EQ(a, 0x00);
}

// Test MOV A, B with 0xFF
TEST_F(CpuTest, MOV_A_B_Full) {
    b = 0xFF;
    MOV_A_B();
    EXPECT_EQ(a, 0xFF);
}

// Test MOV B, A with 0x00
TEST_F(CpuTest, MOV_B_A_Zero) {
    a = 0x00;
    MOV_B_A();
    EXPECT_EQ(b, 0x00);
}

// Test MOV B, A with 0xFF
TEST_F(CpuTest, MOV_B_A_Full) {
    a = 0xFF;
    MOV_B_A();
    EXPECT_EQ(b, 0xFF);
}

// Test MOV A, M when memory contains 0x00
TEST_F(CpuTest, MOV_A_M_Zero) {
    h = 0x20;
    l = 0x10;
    write(0x2010, 0x00);
    MOV_A_M();
    EXPECT_EQ(a, 0x00);
}

// Test MOV A, M when memory contains 0xFF
TEST_F(CpuTest, MOV_A_M_Full) {
    h = 0x20;
    l = 0x10;
    write(0x2010, 0xFF);
    MOV_A_M();
    EXPECT_EQ(a, 0xFF);
}

// Test MOV B, M when memory contains 0x00
TEST_F(CpuTest, MOV_B_M_Zero) {
    h = 0x30;
    l = 0x20;
    write(0x3020, 0x00);
    MOV_B_M();
    EXPECT_EQ(b, 0x00);
}

// Test MOV B, M when memory contains 0xFF
TEST_F(CpuTest, MOV_B_M_Full) {
    h = 0x30;
    l = 0x20;
    write(0x3020, 0xFF);
    MOV_B_M();
    EXPECT_EQ(b, 0xFF);
}

// Test MOV C, D with 0x00
TEST_F(CpuTest, MOV_C_D_Zero) {
    d = 0x00;
    MOV_C_D();
    EXPECT_EQ(c, 0x00);
}

// Test MOV C, D with 0xFF
TEST_F(CpuTest, MOV_C_D_Full) {
    d = 0xFF;
    MOV_C_D();
    EXPECT_EQ(c, 0xFF);
}

// Test MOV C, M when memory contains 0x00
TEST_F(CpuTest, MOV_C_M_Zero) {
    h = 0x40;
    l = 0x30;
    write(0x4030, 0x00);
    MOV_C_M();
    EXPECT_EQ(c, 0x00);
}

// Test MOV C, M when memory contains 0xFF
TEST_F(CpuTest, MOV_C_M_Full) {
    h = 0x40;
    l = 0x30;
    write(0x4030, 0xFF);
    MOV_C_M();
    EXPECT_EQ(c, 0xFF);
}

// Test MOV D, E with 0x00
TEST_F(CpuTest, MOV_D_E_Zero) {
    e = 0x00;
    MOV_D_E();
    EXPECT_EQ(d, 0x00);
}

// Test MOV D, E with 0xFF
TEST_F(CpuTest, MOV_D_E_Full) {
    e = 0xFF;
    MOV_D_E();
    EXPECT_EQ(d, 0xFF);
}

// Test MOV D, M when memory contains 0x00
TEST_F(CpuTest, MOV_D_M_Zero) {
    h = 0x50;
    l = 0x40;
    write(0x5040, 0x00);
    MOV_D_M();
    EXPECT_EQ(d, 0x00);
}

// Test MOV D, M when memory contains 0xFF
TEST_F(CpuTest, MOV_D_M_Full) {
    h = 0x50;
    l = 0x40;
    write(0x5040, 0xFF);
    MOV_D_M();
    EXPECT_EQ(d, 0xFF);
}
