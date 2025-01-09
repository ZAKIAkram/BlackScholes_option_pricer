#include <gtest/gtest.h>
#include <memory>
#include "EuropeanOption.hpp"



class EuropeanOptionTest : public ::testing::Test {
protected:
    std::shared_ptr<Payoff> payoff;
    double maturity = 1.0;
    double sigma = 0.2;
    double rate_0 = 0.05;
    double rate_1 = 0.03;

};

// Test case for the getExerciseType method
TEST_F(EuropeanOptionTest, TestGetExerciseType) {
    EuropeanOption option(payoff, maturity, sigma, rate_0, rate_1);

    // Test if the getExerciseType method returns "European"
    EXPECT_EQ(option.getExerciseType(), "European");
}

// Test case for the clone method
TEST_F(EuropeanOptionTest, TestClone) {
    EuropeanOption option(payoff, maturity, sigma, rate_0, rate_1);
    std::shared_ptr<Option> clonedOption = option.clone();

    // Check if the clone is not null and is of the correct type
    EXPECT_NE(clonedOption, nullptr);
    EXPECT_EQ(clonedOption->getExerciseType(), "European");

    // Optionally, check if the properties of the cloned object match
    EXPECT_DOUBLE_EQ(clonedOption->getMaturity(), maturity);
    EXPECT_DOUBLE_EQ(clonedOption->getSigma(), sigma);
}

