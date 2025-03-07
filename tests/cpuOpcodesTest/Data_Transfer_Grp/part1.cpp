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
