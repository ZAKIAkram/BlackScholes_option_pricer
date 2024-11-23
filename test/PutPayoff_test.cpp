#include <gtest/gtest.h>
#include "PutPayoff.hpp"

TEST(PutPayoffTests, ComputingPayoffExercise) {
    PutPayoff call(110.);
    EXPECT_EQ(call(120.), 0.);
}

TEST(PutPayoffTests, ComputingPayoffNoExercise) {
    PutPayoff call(100.);
    EXPECT_EQ(call(90.), 10.);
}
