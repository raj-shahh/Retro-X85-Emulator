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
