#include <gtest/gtest.h>
#include "BlackScholesExplicit.hpp"
#include "CallPayoff.hpp"
#include "EuropeanOption.hpp"

TEST(BlackScholesExplicitSolverTest, BlackScholesPrice) {
    double S = 100.0;
    double K = 100.0;
    double T = 1.0;
    double r = 0.05;
    double sigma = 0.25;

    auto callPayoff = std::make_shared<CallPayoff>(K);
    EuropeanOption callOption(callPayoff, T, sigma, r, r);

    double price = BlackScholesExplicitSolver::blackScholesPrice(S, callOption);
    EXPECT_NEAR(price, 12.36, 0.1);
}