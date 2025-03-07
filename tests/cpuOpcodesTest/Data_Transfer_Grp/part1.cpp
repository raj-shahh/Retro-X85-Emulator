#include "cpuTestClass.hpp"

TEST_F(CpuTest, MOV_A_B) {
    a = 5; // set accumulator to 5
    b = 10; // set reg b to 10
    MOV_A_B();
    ASSERT_EQ(a, 10);
}