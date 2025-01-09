#include <gtest/gtest.h>
#include <memory>
#include "AmericanOption.hpp"

// Test Fixture class to avoid code duplication
class AmericanOptionTest : public ::testing::Test {
protected:
    std::shared_ptr<Payoff> payoff;
    double maturity = 1.0;
    double sigma = 0.2;
    double rate_0 = 0.05;
    double rate_1 = 0.03;

};

// Test case for constructor and the getExerciseType() method
TEST_F(AmericanOptionTest, TestConstructorAndExerciseType) {
    AmericanOption option(payoff, maturity, sigma, rate_0, rate_1);

    // Check if the exercise type is American
    EXPECT_EQ(option.getExerciseType(), "American");

    // Test if the properties are set correctly
    EXPECT_DOUBLE_EQ(option.getMaturity(), maturity);
    EXPECT_DOUBLE_EQ(option.getSigma(), sigma);
    EXPECT_DOUBLE_EQ(option.getRates().at(0), rate_0);
    EXPECT_DOUBLE_EQ(option.getRates().at(1), rate_1);
}

// Test case for cloning functionality
TEST_F(AmericanOptionTest, TestClone) {
    AmericanOption option(payoff, maturity, sigma, rate_0, rate_1);
    std::shared_ptr<Option> clonedOption = option.clone();

    // Check if the clone is of the correct type
    EXPECT_NE(clonedOption, nullptr);
    EXPECT_EQ(clonedOption->getExerciseType(), "American");

    // Optionally, check if the properties of the cloned object match
    EXPECT_DOUBLE_EQ(clonedOption->getMaturity(), maturity);
    EXPECT_DOUBLE_EQ(clonedOption->getSigma(), sigma);
}


