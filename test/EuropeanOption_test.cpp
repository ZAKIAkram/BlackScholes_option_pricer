#include <gtest/gtest.h>
#include "EuropeanOption.hpp"
#include "CallPayoff.hpp"
#include "PutPayoff.hpp"
#include <memory>

class EuropeanOptionTest : public ::testing::Test {
protected:
    void SetUp() override {
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

TEST_F(EuropeanOptionTest, ConstructorWithCallPayoff) {
    EuropeanOption callOption(callPayoff, maturity, sigma, rate_0, rate_1);

    EXPECT_EQ(callOption.getMaturity(), maturity);
    EXPECT_EQ(callOption.getSigma(), sigma);
    EXPECT_EQ(callOption.getRates().at(0), rate_0);
    EXPECT_EQ(callOption.getRates().at(1), rate_1);
    EXPECT_EQ(callOption.getType(), "call");
    EXPECT_EQ(callOption.getStrike(), strike);
}

TEST_F(EuropeanOptionTest, ConstructorWithPutPayoff) {
    EuropeanOption putOption(putPayoff, maturity, sigma, rate_0, rate_1);

    EXPECT_EQ(putOption.getMaturity(), maturity);
    EXPECT_EQ(putOption.getSigma(), sigma);
    EXPECT_EQ(putOption.getRates().at(0), rate_0);
    EXPECT_EQ(putOption.getRates().at(1), rate_1);
    EXPECT_EQ(putOption.getType(), "put");
    EXPECT_EQ(putOption.getStrike(), strike);
}

TEST_F(EuropeanOptionTest, CopyConstructor) {
    EuropeanOption original(callPayoff, maturity, sigma, rate_0, rate_1);
    EuropeanOption copy(original);

    EXPECT_EQ(copy.getMaturity(), original.getMaturity());
    EXPECT_EQ(copy.getSigma(), original.getSigma());
    EXPECT_EQ(copy.getRates().at(0), original.getRates().at(0));
    EXPECT_EQ(copy.getRates().at(1), original.getRates().at(1));
    EXPECT_EQ(copy.getType(), original.getType());
    EXPECT_EQ(copy.getStrike(), original.getStrike());
}

TEST_F(EuropeanOptionTest, GetExerciseType) {
    EuropeanOption option(callPayoff, maturity, sigma, rate_0, rate_1);
    EXPECT_EQ(option.getExerciseType(), "European");
}

TEST_F(EuropeanOptionTest, DifferentRates) {
    double different_rate_1 = 0.06;
    EuropeanOption option(callPayoff, maturity, sigma, rate_0, different_rate_1);

    EXPECT_EQ(option.getRates().at(0), rate_0);
    EXPECT_EQ(option.getRates().at(1), different_rate_1);
}

TEST_F(EuropeanOptionTest, ZeroMaturity) {
    EuropeanOption option(callPayoff, 0.0, sigma, rate_0, rate_1);
    EXPECT_EQ(option.getMaturity(), 0.0);
}

TEST_F(EuropeanOptionTest, ZeroVolatility) {
    EuropeanOption option(callPayoff, maturity, 0.0, rate_0, rate_1);
    EXPECT_EQ(option.getSigma(), 0.0);
}

TEST_F(EuropeanOptionTest, NegativeRatesHandling) {
    double negative_rate = -0.02;
    EuropeanOption option(callPayoff, maturity, sigma, negative_rate, negative_rate);

    EXPECT_EQ(option.getRates().at(0), negative_rate);
    EXPECT_EQ(option.getRates().at(1), negative_rate);
}

TEST_F(EuropeanOptionTest, PayoffTypeCheck) {
    EuropeanOption callOption(callPayoff, maturity, sigma, rate_0, rate_1);
    EuropeanOption putOption(putPayoff, maturity, sigma, rate_0, rate_1);

    EXPECT_EQ(callOption.getType(), "call");
    EXPECT_EQ(putOption.getType(), "put");
}