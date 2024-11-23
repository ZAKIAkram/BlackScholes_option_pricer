#include <gtest/gtest.h>
#include "CallPayoff.hpp"

TEST(CallPayoffTests, ComputingPayoffExercise) {
    CallPayoff call(110.);
    EXPECT_EQ(call(120.), 10.);
}

TEST(CallPayoffTests, ComputingPayoffNoExercise) {
    CallPayoff call(100.);
    EXPECT_EQ(call(90.), 0.);
}

