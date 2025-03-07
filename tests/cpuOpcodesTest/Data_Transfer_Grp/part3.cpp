#include "cpuTestClass.hpp"

////////////////////////////// MVI Tests //////////////////////////////

TEST_F(CpuTest, MVI_A) {
    fetched_low = 0x42;
    MVI_A();
    EXPECT_EQ(a, 0x42);
}

TEST_F(CpuTest, MVI_B) {
    fetched_low = 0x33;
    MVI_B();
    EXPECT_EQ(b, 0x33);
}

TEST_F(CpuTest, MVI_C) {
    fetched_low = 0x99;
    MVI_C();
    EXPECT_EQ(c, 0x99);
}

TEST_F(CpuTest, MVI_D) {
    fetched_low = 0x55;
    MVI_D();
    EXPECT_EQ(d, 0x55);
}

TEST_F(CpuTest, MVI_E) {
    fetched_low = 0x77;
    MVI_E();
    EXPECT_EQ(e, 0x77);
}

TEST_F(CpuTest, MVI_H) {
    fetched_low = 0x88;
    MVI_H();
    EXPECT_EQ(h, 0x88);
}

TEST_F(CpuTest, MVI_L) {
    fetched_low = 0xAA;
    MVI_L();
    EXPECT_EQ(l, 0xAA);
}

TEST_F(CpuTest, MVI_M) {
    h = 0x20;
    l = 0x30;
    fetched_low = 0x66;
    MVI_M();
    EXPECT_EQ(read(0x2030), 0x66);
}

////////////////////////////// LXI Tests //////////////////////////////

TEST_F(CpuTest, LXI_B) {
    fetched_high = 0x12;
    fetched_low = 0x34;
    LXI_B();
    EXPECT_EQ(b, 0x12);
    EXPECT_EQ(c, 0x34);
}

TEST_F(CpuTest, LXI_D) {
    fetched_high = 0x56;
    fetched_low = 0x78;
    LXI_D();
    EXPECT_EQ(d, 0x56);
    EXPECT_EQ(e, 0x78);
}

TEST_F(CpuTest, LXI_H) {
    fetched_high = 0x9A;
    fetched_low = 0xBC;
    LXI_H();
    EXPECT_EQ(h, 0x9A);
    EXPECT_EQ(l, 0xBC);
}

TEST_F(CpuTest, LXI_SP) {
    fetched_high = 0xDE;
    fetched_low = 0xF0;
    LXI_SP();
    EXPECT_EQ(stkp, 0xDEF0);
}

////////////////////////////// Load Tests //////////////////////////////

TEST_F(CpuTest, LDAX_B) {
    b = 0x12;
    c = 0x34;
    write(0x1234, 0xAB);
    LDAX_B();
    EXPECT_EQ(a, 0xAB);
}

TEST_F(CpuTest, LDAX_D) {
    d = 0x56;
    e = 0x78;
    write(0x5678, 0xCD);
    LDAX_D();
    EXPECT_EQ(a, 0xCD);
}

TEST_F(CpuTest, LHLD) {
    addr_abs = 0x2468;
    write(0x2468, 0x11);
    write(0x2469, 0x22);
    LHLD();
    EXPECT_EQ(l, 0x11);
    EXPECT_EQ(h, 0x22);
}

TEST_F(CpuTest, LDA) {
    addr_abs = 0x1357;
    write(0x1357, 0x44);
    LDA();
    EXPECT_EQ(a, 0x44);
}

////////////////////////////// Store Tests //////////////////////////////

TEST_F(CpuTest, STAX_B) {
    b = 0x12;
    c = 0x34;
    a = 0xEE;
    STAX_B();
    EXPECT_EQ(read(0x1234), 0xEE);
}

TEST_F(CpuTest, STAX_D) {
    d = 0x56;
    e = 0x78;
    a = 0xFF;
    STAX_D();
    EXPECT_EQ(read(0x5678), 0xFF);
}

TEST_F(CpuTest, SHLD) {
    addr_abs = 0x2468;
    l = 0x11;
    h = 0x22;
    SHLD();
    EXPECT_EQ(read(0x2468), 0x11);
    EXPECT_EQ(read(0x2469), 0x22);
}

TEST_F(CpuTest, STA) {
    addr_abs = 0x1357;
    a = 0x88;
    STA();
    EXPECT_EQ(read(0x1357), 0x88);
}
