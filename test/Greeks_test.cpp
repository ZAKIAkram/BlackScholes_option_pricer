#include <gtest/gtest.h>
#include "Greeks.hpp"
#include "BlackScholesExplicit.hpp"
#include "EuropeanOption.hpp"
#include "AmericanOption.hpp"
#include "CallPayoff.hpp"
#include "PutPayoff.hpp"

class GreeksTest : public ::testing::Test {
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

        callGrid = CrankNicolsonSolver::solve(*europeanCall, N, M);
        putGrid = CrankNicolsonSolver::solve(*europeanPut, N, M);

        callGreeks = std::make_unique<Greeks>(*europeanCall, callGrid, N, M);
        putGreeks = std::make_unique<Greeks>(*europeanPut, putGrid, N, M);
    }

    double S, K, T, r, sigma;
    int M, N;
    double tolerance;
    std::shared_ptr<CallPayoff> callPayoff;
    std::shared_ptr<PutPayoff> putPayoff;
    std::unique_ptr<EuropeanOption> europeanCall;
    std::unique_ptr<EuropeanOption> europeanPut;
    std::vector<std::vector<double>> callGrid;
    std::vector<std::vector<double>> putGrid;
    std::unique_ptr<Greeks> callGreeks;
    std::unique_ptr<Greeks> putGreeks;

    int getIndex(double spot) {
        return static_cast<int>(spot * M / (2 * K));
    }
};

TEST_F(GreeksTest, DeltaBounds) {
    int idx = getIndex(S);
    // Call delta should be between 0 and 1
    EXPECT_GE(callGreeks->getDelta()[idx], 0.0);
    EXPECT_LE(callGreeks->getDelta()[idx], 1.0);
    // Put delta should be between -1 and 0
    EXPECT_GE(putGreeks->getDelta()[idx], -1.0);
    EXPECT_LE(putGreeks->getDelta()[idx], 0.0);
}

// Test Put-Call Parity for Delta
TEST_F(GreeksTest, PutCallParityDelta) {
    int idx = getIndex(S);
    double callDelta = callGreeks->getDelta()[idx];
    double putDelta = putGreeks->getDelta()[idx];
    EXPECT_NEAR(callDelta - putDelta, 1.0, tolerance);
}

// Test Gamma positivity and equality for puts and calls
TEST_F(GreeksTest, GammaProperties) {
    int idx = getIndex(S);
    double callGamma = callGreeks->getGamma()[idx];
    double putGamma = putGreeks->getGamma()[idx];
    // Gamma should be positive
    EXPECT_GT(callGamma, 0.0);
    EXPECT_GT(putGamma, 0.0);
    // Call and put gamma should be equal
    EXPECT_NEAR(callGamma, putGamma, tolerance);
}

TEST_F(GreeksTest, VegaProperties) {
    int idx = getIndex(S);
    double callVega = callGreeks->getVega()[idx];
    double putVega = putGreeks->getVega()[idx];
    // Vega should be positive
    EXPECT_GT(callVega, 0.0);
    EXPECT_GT(putVega, 0.0);
    // Call and put vega should be equal
    EXPECT_NEAR(callVega, putVega, tolerance);
}


TEST_F(GreeksTest, ThetaProperties) {
    int idx = getIndex(S);
    // For at-the-money options with positive interest rate
    if (std::abs(S - K) < 1e-6 && r > 0) {
        // Call theta is negative for ATM options
        EXPECT_LT(callGreeks->getTheta()[idx][0], 0.0);
    }
    // iTM puts have positive theta
    int deepItmIdx = getIndex(0.5 * K);
    EXPECT_GT(putGreeks->getTheta()[deepItmIdx][0], 0.0);
}

TEST_F(GreeksTest, RhoDirectionalProperties) {
    int idx = getIndex(S);
    double callRho = callGreeks->getRho()[idx];
    double putRho = putGreeks->getRho()[idx];
    EXPECT_GT(callRho, 0.0);
    EXPECT_LT(putRho, 0.0);
}

TEST_F(GreeksTest, BoundaryConditions) {
    EXPECT_NEAR(callGreeks->getDelta()[0], 0.0, tolerance);
    EXPECT_NEAR(callGreeks->getGamma()[0], 0.0, tolerance);

    EXPECT_NEAR(callGreeks->getDelta()[M], 1.0, tolerance);
    EXPECT_NEAR(callGreeks->getGamma()[M], 0.0, tolerance);
}
