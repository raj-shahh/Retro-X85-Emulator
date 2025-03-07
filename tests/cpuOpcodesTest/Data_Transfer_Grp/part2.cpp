#include "cpuTestClass.hpp"

TEST_F(CpuTest, MOV_E_Register) {
    a = 0x42;
    MOV_E_A();
    EXPECT_EQ(e, 0x42);
}

TEST_F(CpuTest, MOV_E_Memory) {
    h = 0x10;
    l = 0x20;
    write(0x1020, 0x88);
    MOV_E_M();
    EXPECT_EQ(e, 0x88);
}

TEST_F(CpuTest, MOV_H_Register) {
    b = 0x34;
    MOV_H_B();
    EXPECT_EQ(h, 0x34);
}

TEST_F(CpuTest, MOV_H_Memory) {
    h = 0x20;
    l = 0x30;
    write(0x2030, 0x77);
    MOV_H_M();
    EXPECT_EQ(h, 0x77);
}

TEST_F(CpuTest, MOV_L_Register) {
    c = 0x99;
    MOV_L_C();
    EXPECT_EQ(l, 0x99);
}

TEST_F(CpuTest, MOV_L_Memory) {
    h = 0x30;
    l = 0x40;
    write(0x3040, 0x55);
    MOV_L_M();
    EXPECT_EQ(l, 0x55);
}

TEST_F(CpuTest, MOV_Memory_A) {
    h = 0x40;
    l = 0x50;
    a = 0x22;
    MOV_M_A();
    EXPECT_EQ(read(0x4050), 0x22);
}

TEST_F(CpuTest, MOV_Memory_B) {
    h = 0x50;
    l = 0x60;
    b = 0x11;
    MOV_M_B();
    EXPECT_EQ(read(0x5060), 0x11);
}

TEST_F(CpuTest, MOV_Memory_C) {
    h = 0x60;
    l = 0x70;
    c = 0x77;
    MOV_M_C();
    EXPECT_EQ(read(0x6070), 0x77);
}

TEST_F(CpuTest, MOV_Memory_D) {
    h = 0x70;
    l = 0x80;
    d = 0x44;
    MOV_M_D();
    EXPECT_EQ(read(0x7080), 0x44);
}

TEST_F(CpuTest, MOV_Memory_E) {
    h = 0x80;
    l = 0x90;
    e = 0x66;
    MOV_M_E();
    EXPECT_EQ(read(0x8090), 0x66);
}

TEST_F(CpuTest, MOV_Memory_H) {
    h = 0x90;
    l = 0xA0;
    MOV_M_H();
    EXPECT_EQ(read(0x90A0), 0x90);
}

TEST_F(CpuTest, MOV_Memory_L) {
    h = 0xA0;
    l = 0xB0;
    MOV_M_L();
    EXPECT_EQ(read(0xA0B0), 0xB0);
}

TEST_F(CpuTest, XCHG) {
    d = 0x12;
    e = 0x34;
    h = 0x56;
    l = 0x78;
    XCHG();
    EXPECT_EQ(d, 0x56);
    EXPECT_EQ(e, 0x78);
    EXPECT_EQ(h, 0x12);
    EXPECT_EQ(l, 0x34);
}



// ------------------------- MOV Edge Cases -------------------------

// Test MOV E, E (Self Assignment)
TEST_F(CpuTest, MOV_E_E) {
    e = 0x55;
    MOV_E_E();
    EXPECT_EQ(e, 0x55);
}

// Test MOV E, A with 0x00
TEST_F(CpuTest, MOV_E_A_Zero) {
    a = 0x00;
    MOV_E_A();
    EXPECT_EQ(e, 0x00);
}

// Test MOV E, A with 0xFF
TEST_F(CpuTest, MOV_E_A_Full) {
    a = 0xFF;
    MOV_E_A();
    EXPECT_EQ(e, 0xFF);
}

// Test MOV H, H (Self Assignment)
TEST_F(CpuTest, MOV_H_H) {
    h = 0x77;
    MOV_H_H();
    EXPECT_EQ(h, 0x77);
}

// Test MOV H, B with 0x00
TEST_F(CpuTest, MOV_H_B_Zero) {
    b = 0x00;
    MOV_H_B();
    EXPECT_EQ(h, 0x00);
}

// Test MOV H, B with 0xFF
TEST_F(CpuTest, MOV_H_B_Full) {
    b = 0xFF;
    MOV_H_B();
    EXPECT_EQ(h, 0xFF);
}

// Test MOV L, C with 0x00
TEST_F(CpuTest, MOV_L_C_Zero) {
    c = 0x00;
    MOV_L_C();
    EXPECT_EQ(l, 0x00);
}

// Test MOV L, C with 0xFF
TEST_F(CpuTest, MOV_L_C_Full) {
    c = 0xFF;
    MOV_L_C();
    EXPECT_EQ(l, 0xFF);
}

// Test MOV M, A when A is 0x00
TEST_F(CpuTest, MOV_M_A_Zero) {
    h = 0x40;
    l = 0x50;
    a = 0x00;
    MOV_M_A();
    EXPECT_EQ(read(0x4050), 0x00);
}

// Test MOV M, A when A is 0xFF
TEST_F(CpuTest, MOV_M_A_Full) {
    h = 0x40;
    l = 0x50;
    a = 0xFF;
    MOV_M_A();
    EXPECT_EQ(read(0x4050), 0xFF);
}

// Test MOV M, E when E is 0x00
TEST_F(CpuTest, MOV_M_E_Zero) {
    h = 0x80;
    l = 0x90;
    e = 0x00;
    MOV_M_E();
    EXPECT_EQ(read(0x8090), 0x00);
}

// Test MOV M, E when E is 0xFF
TEST_F(CpuTest, MOV_M_E_Full) {
    h = 0x80;
    l = 0x90;
    e = 0xFF;
    MOV_M_E();
    EXPECT_EQ(read(0x8090), 0xFF);
}

// Test MOV M, H when H is 0x90
TEST_F(CpuTest, MOV_M_H_Zero) {
    h = 0x90;
    l = 0xA0;
    MOV_M_H();
    EXPECT_EQ(read(0x90A0), 0x90);
}

// Test MOV M, H when H is 0xFF
TEST_F(CpuTest, MOV_M_H_Full) {
    h = 0xFF;
    l = 0xA0;
    MOV_M_H();
    EXPECT_EQ(read(0xFFA0), 0xFF);
}

// ------------------------- XCHG Edge Cases -------------------------

// Test XCHG with 0x00 and 0xFF
TEST_F(CpuTest, XCHG_Zero_Full) {
    d = 0x00;
    e = 0xFF;
    h = 0xFF;
    l = 0x00;
    XCHG();
    EXPECT_EQ(d, 0xFF);
    EXPECT_EQ(e, 0x00);
    EXPECT_EQ(h, 0x00);
    EXPECT_EQ(l, 0xFF);
}

// Test XCHG with same values in DE and HL
TEST_F(CpuTest, XCHG_SameValues) {
    d = 0x12;
    e = 0x34;
    h = 0x12;
    l = 0x34;
    XCHG();
    EXPECT_EQ(d, 0x12);
    EXPECT_EQ(e, 0x34);
    EXPECT_EQ(h, 0x12);
    EXPECT_EQ(l, 0x34);
}

// Test XCHG with H=L and D=E (Invalid but should not crash)
TEST_F(CpuTest, XCHG_HL_DE_Same) {
    d = 0x56;
    e = 0x56;
    h = 0x56;
    l = 0x56;
    XCHG();
    EXPECT_EQ(d, 0x56);
    EXPECT_EQ(e, 0x56);
    EXPECT_EQ(h, 0x56);
    EXPECT_EQ(l, 0x56);
}

