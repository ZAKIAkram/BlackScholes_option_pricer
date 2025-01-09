#include <gtest/gtest.h>
#include "Rates.hpp"


// Test for constructor and value at t = 0 and t = 1
TEST(RatesTest, BasicInterpolation) {
    Rates r(2.0, 5.0);
    EXPECT_DOUBLE_EQ(r.at(0.0), 2.0);
    EXPECT_DOUBLE_EQ(r.at(1.0), 5.0);
}

// Test for linear interpolation at t = 0.5
TEST(RatesTest, MidPointInterpolation) {
    Rates r(2.0, 6.0);
    EXPECT_DOUBLE_EQ(r.at(0.5), 4.0);
}

// Test for value at t = 0.25 and t = 0.75
TEST(RatesTest, QuarterInterpolation) {
    Rates r(1.0, 5.0);
    EXPECT_DOUBLE_EQ(r.at(0.25), 2.0);
    EXPECT_DOUBLE_EQ(r.at(0.75), 4.0);
}

// Test for exception when t is out of bounds
TEST(RatesTest, OutOfRangeException) {
    Rates r(3.0, 7.0);
    EXPECT_THROW(r.at(-0.1), std::out_of_range);
    EXPECT_THROW(r.at(1.1), std::out_of_range);
}

// Test for copy constructor
TEST(RatesTest, CopyConstructor) {
    Rates r1(2.5, 4.5);
    Rates r2(r1);
    EXPECT_DOUBLE_EQ(r2.at(0.0), 2.5);
    EXPECT_DOUBLE_EQ(r2.at(1.0), 4.5);
    EXPECT_DOUBLE_EQ(r2.at(0.5), 3.5);
}

