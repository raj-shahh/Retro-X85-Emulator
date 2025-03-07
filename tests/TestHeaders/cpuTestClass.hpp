#pragma once

#include <gtest/gtest.h>
#include "cpu8085.h"  // Include the class to test

class CpuTest : public cpu8085, public ::testing::Test {  // can access all pub and proc members of cpu8085
protected:
    void SetUp() override {
        // Any setup needed before tests run
    }

    void TearDown() override {
        // Any cleanup after each test
    }
};


