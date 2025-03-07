#include "cpuTestClass.hpp"

TEST_F(CpuTest, AddRegisters_ADD_B) {
    a = 5; // set accumulator to 5
    b = 3; // set reg b to 3
    ADD_B();
    ASSERT_EQ(a, 8) << "ADD_B() failed \n";  // Verify the addition
}