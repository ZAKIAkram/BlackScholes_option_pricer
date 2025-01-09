#include <gtest/gtest.h>
#include "BlackScholesExplicit.hpp"

#include <cmath>

#include "EuropeanOption.hpp"
#include "AmericanOption.hpp"
#include "Payoff.hpp"
#include "CallPayoff.hpp"
#include "PutPayoff.hpp"
#include "CrankNicolsonSolver.hpp"



class BlackScholesTest : public ::testing::Test {
protected:
    void SetUp() override {
        S = 100.0;  // Current price
        K = 100.0;  // Strike price
        T = 1.0;    // Time to maturity
        r = 0.05;   // Risk-free rate
        sigma = 0.2; // Volatility
    }

    double S, K, T, r, sigma;
};



TEST_F(BlackScholesTest, AtTheMoneyCallOption) {
    auto callPayoff = std::make_shared<CallPayoff>(K);
    EuropeanOption callOption(callPayoff, T, sigma, r, r);

    double price = BlackScholesExplicitSolver::blackScholesPrice(S, callOption);

    // Expected price calculated using standard Black-Scholes formula
    // for ATM call option with given parameters
    double expectedPrice = 10.45; // Pre-calculated value
    EXPECT_NEAR(price, expectedPrice, 0.01);
}

TEST_F(BlackScholesTest, AtTheMoneyPutOption) {
    auto putPayoff = std::make_shared<PutPayoff>(K);
    EuropeanOption putOption(putPayoff, T, sigma, r, r);

    double price = BlackScholesExplicitSolver::blackScholesPrice(S, putOption);

    // Expected price calculated using put-call parity
    double expectedPrice = 5.57; // Pre-calculated value
    EXPECT_NEAR(price, expectedPrice, 0.01);
}

TEST_F(BlackScholesTest, DeepOutOfTheMoneyCall) {
    S = 50.0;  // Spot price much lower than strike
    auto callPayoff = std::make_shared<CallPayoff>(K);
    EuropeanOption callOption(callPayoff, T, sigma, r, r);

    double price = BlackScholesExplicitSolver::blackScholesPrice(S, callOption);

    // Deep OTM call should have very low value
    EXPECT_LT(price, 0.5);
}

TEST_F(BlackScholesTest, DeepInTheMoneyPut) {
    S = 50.0;  // Spot price much lower than strike
    auto putPayoff = std::make_shared<PutPayoff>(K);
    EuropeanOption putOption(putPayoff, T, sigma, r, r);

    double price = BlackScholesExplicitSolver::blackScholesPrice(S, putOption);

    // Deep ITM put should be approximately K*exp(-rT) - S
    double intrinsicValue = K * std::exp(-r * T) - S;
    EXPECT_NEAR(price, intrinsicValue, 5.0);
}

TEST_F(BlackScholesTest, ZeroVolatility) {
    sigma = 0.0;
    auto callPayoff = std::make_shared<CallPayoff>(K);
    EuropeanOption callOption(callPayoff, T, sigma, r, r);

    double price = BlackScholesExplicitSolver::blackScholesPrice(S, callOption);

    // With zero volatility, call option should be worth max(0, S - K*exp(-rT))
    double expectedPrice = std::max(0.0, S - K * std::exp(-r * T));
    EXPECT_NEAR(price, expectedPrice, 0.01);
}

TEST_F(BlackScholesTest, PutCallParity) {
    auto callPayoff = std::make_shared<CallPayoff>(K);
    auto putPayoff = std::make_shared<PutPayoff>(K);

    EuropeanOption callOption(callPayoff, T, sigma, r, r);
    EuropeanOption putOption(putPayoff, T, sigma, r, r);

    double callPrice = BlackScholesExplicitSolver::blackScholesPrice(S, callOption);
    double putPrice = BlackScholesExplicitSolver::blackScholesPrice(S, putOption);

    // Put-Call parity: C + K*exp(-rT) = P + S
    double leftSide = callPrice + K * std::exp(-r * T);
    double rightSide = putPrice + S;
    EXPECT_NEAR(leftSide, rightSide, 0.01);
}

