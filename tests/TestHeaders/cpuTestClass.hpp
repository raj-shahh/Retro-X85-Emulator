#pragma once

#include <gtest/gtest.h>
#include "cpu8085.h"  // Include the class to test


class CpuTest : public cpu8085, public ::testing::Test {  // can access all pub and proc members of cpu8085
protected:
    std::array<uint8_t, 64 * 1024> fakeram;

    void write(uint16_t a , uint8_t d) override{
        fakeram[a]=d;
    }

    uint8_t read(uint16_t a) override{
        return fakeram[a];
    }

protected:

    //Runs before every test;
    void SetUp() override {
        // Initialize CPU registers before each test
        a = b = c = d = e = h = l = 0;
        status = 0;
        stkp = 0x1000;
        pc = 0;
    }

    //Runs after every test;
    void TearDown() override {
        // Any cleanup after each test
        ;
    }
};


