#include "cpuTestClass.hpp"

TEST_F( CpuTest, ANA_Register)
{
    a = 0xF0;
    b = 0x0F;
    ANA_B();
    EXPECT_EQ(a, 0x00);
    EXPECT_TRUE(GetFlag(cpu8085::Z));
    EXPECT_FALSE(GetFlag(cpu8085::C));
    EXPECT_TRUE(GetFlag(cpu8085::A));
}

TEST_F( CpuTest, ANA_Memory)
{
    a = 0xF0;
    h = 0x20;
    l = 0x10;
    write(0x2010, 0x0F);
    ANA_M();
    EXPECT_EQ(a, 0x00);
    EXPECT_TRUE(GetFlag(cpu8085::Z));
}

TEST_F( CpuTest, ANI_Immediate)
{
    a = 0xF0;
    fetched_low = 0x0F;
    ANI();
    EXPECT_EQ(a, 0x00);
}

TEST_F( CpuTest, XRA_Register)
{
    a = 0xFF;
    b = 0xFF;
    XRA_B();
    EXPECT_EQ(a, 0x00);
    EXPECT_TRUE(GetFlag(cpu8085::Z));
}

TEST_F( CpuTest, ORA_Register)
{
    a = 0xF0;
    b = 0x0F;
    ORA_B();
    EXPECT_EQ(a, 0xFF);
    EXPECT_FALSE(GetFlag(cpu8085::Z));
}

TEST_F( CpuTest, CMP_Register)
{
    a = 0x10;
    b = 0x20;
    CMP_B();
    EXPECT_TRUE(GetFlag(cpu8085::C));
    EXPECT_TRUE(GetFlag(cpu8085::S));
}

TEST_F( CpuTest, CMP_Memory)
{
    a = 0x10;
    h = 0x20;
    l = 0x10;
    write(0x2010, 0x10);
    CMP_M();
    EXPECT_TRUE(GetFlag(cpu8085::Z));
}

TEST_F( CpuTest, CPI_Immediate)
{
    a = 0x10;
    fetched_low = 0x20;
    CPI();
    EXPECT_TRUE(GetFlag(cpu8085::C));
}



// ------------------------- ANA Edge Cases -------------------------

// Test ANA with itself
TEST_F( CpuTest, ANA_A)
{
    a = 0xF0;
    ANA_A();
    EXPECT_EQ(a, 0xF0);
    EXPECT_FALSE(GetFlag(cpu8085::Z));
}

// Test ANA with 0xFF (should remain unchanged)
TEST_F( CpuTest, ANA_B_FullMask)
{
    a = 0xAA;
    b = 0xFF;
    ANA_B();
    EXPECT_EQ(a, 0xAA);
    EXPECT_FALSE(GetFlag(cpu8085::Z));
}

// Test ANA with 0x00 (should always be zero)
TEST_F( CpuTest, ANA_B_Zero)
{
    a = 0xAA;
    b = 0x00;
    ANA_B();
    EXPECT_EQ(a, 0x00);
    EXPECT_TRUE(GetFlag(cpu8085::Z));
}

// ------------------------- ANI Edge Cases -------------------------

// Test ANI with 0xFF (should remain unchanged)
TEST_F( CpuTest, ANI_FullMask)
{
    a = 0xAA;
    fetched_low = 0xFF;
    ANI();
    EXPECT_EQ(a, 0xAA);
    EXPECT_FALSE(GetFlag(cpu8085::Z));
}

// Test ANI with 0x00 (should always be zero)
TEST_F( CpuTest, ANI_ZeroMask)
{
    a = 0xAA;
    fetched_low = 0x00;
    ANI();
    EXPECT_EQ(a, 0x00);
    EXPECT_TRUE(GetFlag(cpu8085::Z));
}

// ------------------------- XRA Edge Cases -------------------------

// Test XRA with itself (should always be zero)
TEST_F( CpuTest, XRA_A)
{
    a = 0xF0;
    XRA_A();
    EXPECT_EQ(a, 0x00);
    EXPECT_TRUE(GetFlag(cpu8085::Z));
}

// Test XRA with 0x00 (should remain unchanged)
TEST_F( CpuTest, XRA_B_Zero)
{
    a = 0xAA;
    b = 0x00;
    XRA_B();
    EXPECT_EQ(a, 0xAA);
}

// ------------------------- ORA Edge Cases -------------------------

// Test ORA with itself (should remain unchanged)
TEST_F( CpuTest, ORA_A)
{
    a = 0xAA;
    ORA_A();
    EXPECT_EQ(a, 0xAA);
    EXPECT_FALSE(GetFlag(cpu8085::Z));
}

// Test ORA with 0x00 (should remain unchanged)
TEST_F( CpuTest, ORA_B_Zero)
{
    a = 0xAA;
    b = 0x00;
    ORA_B();
    EXPECT_EQ(a, 0xAA);
}

// ------------------------- CMP Edge Cases -------------------------

// Test CMP with itself (should set Zero flag)
TEST_F( CpuTest, CMP_B_Same)
{
    a = 0x20;
    b = 0x20;
    CMP_B();
    EXPECT_TRUE(GetFlag(cpu8085::Z));
}

// Test CMP when A > operand (should clear Carry)
TEST_F( CpuTest, CMP_B_A_Greater)
{
    a = 0x30;
    b = 0x20;
    CMP_B();
    EXPECT_FALSE(GetFlag(cpu8085::C));
    EXPECT_FALSE(GetFlag(cpu8085::Z));
}

// Test CMP when A < operand (should set Carry and Sign flags)
TEST_F( CpuTest, CMP_B_A_Less)
{
    a = 0x10;
    b = 0x20;
    CMP_B();
    EXPECT_TRUE(GetFlag(cpu8085::C));
    EXPECT_TRUE(GetFlag(cpu8085::S));
}

// ------------------------- CPI Edge Cases -------------------------

// Test CPI with same value (should set Zero flag)
TEST_F( CpuTest, CPI_SameValue)
{
    a = 0x30;
    fetched_low = 0x30;
    CPI();
    EXPECT_TRUE(GetFlag(cpu8085::Z));
}

// Test CPI when A > immediate (should clear Carry)
TEST_F( CpuTest, CPI_A_Greater)
{
    a = 0x40;
    fetched_low = 0x20;
    CPI();
    EXPECT_FALSE(GetFlag(cpu8085::C));
}

// Test CPI when A < immediate (should set Carry and Sign flags)
TEST_F( CpuTest, CPI_A_Less)
{
    a = 0x10;
    fetched_low = 0x20;
    CPI();
    EXPECT_TRUE(GetFlag(cpu8085::C));
    EXPECT_TRUE(GetFlag(cpu8085::S));
}
