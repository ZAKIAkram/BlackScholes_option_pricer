#include <gtest/gtest.h>
#include "CrankNicolsonSolver.hpp"
#include "EuropeanOption.hpp"
#include "AmericanOption.hpp"
#include "CallPayoff.hpp"
#include "PutPayoff.hpp"
#include "BlackScholesExplicit.hpp"
#include "Greeks.hpp"

class CrankNicolsonTest : public ::testing::Test {
protected:
    void SetUp() override {
        S = 100.0;
        K = 100.0;
        T = 1.0;
        r = 0.05;
        sigma = 0.25;
        M = 100;
        N = 100;
        tolerance = 1e-2;  

        callPayoff = std::make_shared<CallPayoff>(K);
        putPayoff = std::make_shared<PutPayoff>(K);


        europeanCall = std::make_unique<EuropeanOption>(callPayoff, T, sigma, r, r);
        europeanPut = std::make_unique<EuropeanOption>(putPayoff, T, sigma, r, r);
        americanCall = std::make_unique<AmericanOption>(callPayoff, T, sigma, r, r);
        americanPut = std::make_unique<AmericanOption>(putPayoff, T, sigma, r, r);
    }

    double S, K, T, r, sigma;
    int M, N;
    double tolerance;
    std::shared_ptr<CallPayoff> callPayoff;
    std::shared_ptr<PutPayoff> putPayoff;
    std::unique_ptr<EuropeanOption> europeanCall;
    std::unique_ptr<EuropeanOption> europeanPut;
    std::unique_ptr<AmericanOption> americanCall;
    std::unique_ptr<AmericanOption> americanPut;

    double getOptionPrice(const std::vector<std::vector<double>>& grid) {
        int S_index = static_cast<int>(S * M / (2 * K));
        return grid[S_index][0];
    }
};


TEST_F(CrankNicolsonTest, EuropeanCallMatchesBlackScholes) {
    auto grid = CrankNicolsonSolver::solve(*europeanCall, N, M);
    double crankPrice = getOptionPrice(grid);
    double bsPrice = BlackScholesExplicitSolver::blackScholesPrice(S, *europeanCall);

    EXPECT_NEAR(crankPrice, bsPrice, tolerance)
        << "Crank-Nicolson price: " << crankPrice
        << ", Black-Scholes price: " << bsPrice;
}


TEST_F(CrankNicolsonTest, EuropeanPutMatchesBlackScholes) {
    auto grid = CrankNicolsonSolver::solve(*europeanPut, N, M);
    double crankPrice = getOptionPrice(grid);
    double bsPrice = BlackScholesExplicitSolver::blackScholesPrice(S, *europeanPut);

    EXPECT_NEAR(crankPrice, bsPrice, tolerance)
        << "Crank-Nicolson price: " << crankPrice
        << ", Black-Scholes price: " << bsPrice;
}


TEST_F(CrankNicolsonTest, AmericanCallEqualsEuropeanCall) {
    auto europeanGrid = CrankNicolsonSolver::solve(*europeanCall, N, M);
    auto americanGrid = CrankNicolsonSolver::solve(*americanCall, N, M);

    double europeanPrice = getOptionPrice(europeanGrid);
    double americanPrice = getOptionPrice(americanGrid);

    EXPECT_NEAR(americanPrice, europeanPrice, tolerance)
        << "American Call price: " << americanPrice
        << ", European Call price: " << europeanPrice;
}


TEST_F(CrankNicolsonTest, AmericanPutGreaterThanEuropeanPut) {
    auto europeanGrid = CrankNicolsonSolver::solve(*europeanPut, N, M);
    auto americanGrid = CrankNicolsonSolver::solve(*americanPut, N, M);

    double europeanPrice = getOptionPrice(europeanGrid);
    double americanPrice = getOptionPrice(americanGrid);

    EXPECT_GE(americanPrice, europeanPrice)
        << "American Put price: " << americanPrice
        << ", European Put price: " << europeanPrice;
}


TEST_F(CrankNicolsonTest, OptionPricesArePositive) {
    auto europeanCallGrid = CrankNicolsonSolver::solve(*europeanCall, N, M);
    auto europeanPutGrid = CrankNicolsonSolver::solve(*europeanPut, N, M);
    auto americanCallGrid = CrankNicolsonSolver::solve(*americanCall, N, M);
    auto americanPutGrid = CrankNicolsonSolver::solve(*americanPut, N, M);

    EXPECT_GT(getOptionPrice(europeanCallGrid), 0.0);
    EXPECT_GT(getOptionPrice(europeanPutGrid), 0.0);
    EXPECT_GT(getOptionPrice(americanCallGrid), 0.0);
    EXPECT_GT(getOptionPrice(americanPutGrid), 0.0);
}
