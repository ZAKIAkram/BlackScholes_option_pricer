#include <gtest/gtest.h>
#include "AmericanOption.hpp"
#include "CallPayoff.hpp"
#include "PutPayoff.hpp"
#include <memory>

class AmericanOptionTest : public ::testing::Test {
protected:
    void SetUp() override {
        //parameters
        maturity = 1.0;
        sigma = 0.2;
        rate_0 = 0.05;
        rate_1 = 0.05;
        strike = 100.0;

        //payoff objects
        callPayoff = std::make_shared<CallPayoff>(strike);
        putPayoff = std::make_shared<PutPayoff>(strike);
    }

    double maturity, sigma, rate_0, rate_1, strike;
    std::shared_ptr<Payoff> callPayoff;
    std::shared_ptr<Payoff> putPayoff;
};

TEST_F(AmericanOptionTest, ConstructorWithCallPayoff) {
    AmericanOption callOption(callPayoff, maturity, sigma, rate_0, rate_1);

    EXPECT_EQ(callOption.getMaturity(), maturity);
    EXPECT_EQ(callOption.getSigma(), sigma);
    EXPECT_EQ(callOption.getRates().at(0), rate_0);
    EXPECT_EQ(callOption.getRates().at(1), rate_1);
    EXPECT_EQ(callOption.getType(), "call");
    EXPECT_EQ(callOption.getStrike(), strike);
    EXPECT_EQ(callOption.getExerciseType(), "American");
}

TEST_F(AmericanOptionTest, ConstructorWithPutPayoff) {
    AmericanOption putOption(putPayoff, maturity, sigma, rate_0, rate_1);

    EXPECT_EQ(putOption.getMaturity(), maturity);
    EXPECT_EQ(putOption.getSigma(), sigma);
    EXPECT_EQ(putOption.getRates().at(0), rate_0);
    EXPECT_EQ(putOption.getRates().at(1), rate_1);
    EXPECT_EQ(putOption.getType(), "put");
    EXPECT_EQ(putOption.getStrike(), strike);
    EXPECT_EQ(putOption.getExerciseType(), "American");
}

TEST_F(AmericanOptionTest, CopyConstructor) {
    AmericanOption original(callPayoff, maturity, sigma, rate_0, rate_1);
    AmericanOption copy(original);

    EXPECT_EQ(copy.getMaturity(), original.getMaturity());
    EXPECT_EQ(copy.getSigma(), original.getSigma());
    EXPECT_EQ(copy.getRates().at(0), original.getRates().at(0));
    EXPECT_EQ(copy.getRates().at(1), original.getRates().at(1));
    EXPECT_EQ(copy.getType(), original.getType());
    EXPECT_EQ(copy.getStrike(), original.getStrike());
    EXPECT_EQ(copy.getExerciseType(), original.getExerciseType());
}

TEST_F(AmericanOptionTest, DifferentRates) {
    double different_rate_1 = 0.06;
    AmericanOption option(callPayoff, maturity, sigma, rate_0, different_rate_1);

    EXPECT_EQ(option.getRates().at(0), rate_0);
    EXPECT_EQ(option.getRates().at(1), different_rate_1);
}

TEST_F(AmericanOptionTest, ExtremeMaturityValues) {
    // Test with very short maturity
    AmericanOption shortOption(callPayoff, 1.0 / 365.0, sigma, rate_0, rate_1);
    EXPECT_NEAR(shortOption.getMaturity(), 1.0 / 365.0, 1e-10);

    // Test with very long maturity
    AmericanOption longOption(callPayoff, 30.0, sigma, rate_0, rate_1);
    EXPECT_EQ(longOption.getMaturity(), 30.0);
}

TEST_F(AmericanOptionTest, ExtremeVolatilityValues) {
    // Test with very low volatility
    AmericanOption lowVolOption(callPayoff, maturity, 0.01, rate_0, rate_1);
    EXPECT_EQ(lowVolOption.getSigma(), 0.01);

    // Test with high volatility
    AmericanOption highVolOption(callPayoff, maturity, 1.0, rate_0, rate_1);
    EXPECT_EQ(highVolOption.getSigma(), 1.0);
}

TEST_F(AmericanOptionTest, ExtremeRateValues) {
    // Test with negative rates)
    AmericanOption negRateOption(callPayoff, maturity, sigma, -0.01, -0.01);
    EXPECT_EQ(negRateOption.getRates().at(0), -0.01);
    EXPECT_EQ(negRateOption.getRates().at(1), -0.01);

    // Test with high rates
    AmericanOption highRateOption(callPayoff, maturity, sigma, 0.15, 0.15);
    EXPECT_EQ(highRateOption.getRates().at(0), 0.15);
    EXPECT_EQ(highRateOption.getRates().at(1), 0.15);
}

TEST_F(AmericanOptionTest, PayoffTypeVerification) {
    AmericanOption callOption(callPayoff, maturity, sigma, rate_0, rate_1);
    AmericanOption putOption(putPayoff, maturity, sigma, rate_0, rate_1);

    EXPECT_EQ(callOption.getType(), "call");
    EXPECT_EQ(putOption.getType(), "put");
}

TEST_F(AmericanOptionTest, SharedPayoffHandling) {
    AmericanOption original(callPayoff, maturity, sigma, rate_0, rate_1);
    AmericanOption copy(original);

    EXPECT_EQ(original.getType(), copy.getType());
    EXPECT_EQ(original.getStrike(), copy.getStrike());
}