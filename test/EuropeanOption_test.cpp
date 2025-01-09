#include <gtest/gtest.h>
#include "Option.hpp"
#include "EuropeanOption.hpp"
#include "AmericanOption.hpp"
#include "CallPayoff.hpp"
#include "PutPayoff.hpp"
#include "CrankNicolsonSolver.hpp"
#include "Greeks.hpp"
#include "BlackScholesExplicit.hpp"

class OptionTest : public ::testing::Test {
protected:
    void SetUp() override {
        S = 100.0;
        K = 100.0;
        T = 1.0;
        r = 0.05;
        sigma = 0.25;
        M = 100;
        N = 100;

        callPayoff = std::make_shared<CallPayoff>(K);
        putPayoff = std::make_shared<PutPayoff>(K);
    }

    double S, K, T, r, sigma;
    int M, N;
    std::shared_ptr<CallPayoff> callPayoff;
    std::shared_ptr<PutPayoff> putPayoff;
};

TEST_F(OptionTest, TestEuropeanOptionConstruction) {
    EuropeanOption callOption(callPayoff, T, sigma, r, r);

    EXPECT_DOUBLE_EQ(callOption.getMaturity(), T);
    EXPECT_DOUBLE_EQ(callOption.getSigma(), sigma);
    EXPECT_DOUBLE_EQ(callOption.getRates().at(0), r);
    EXPECT_DOUBLE_EQ(callOption.getRates().at(1), r);
    EXPECT_EQ(callOption.getExerciseType(), "European");
}

TEST_F(OptionTest, TestEuropeanOptionCopyConstruction) {
    EuropeanOption original(callPayoff, T, sigma, r, r);
    EuropeanOption copy(original);

    EXPECT_DOUBLE_EQ(copy.getMaturity(), original.getMaturity());
    EXPECT_DOUBLE_EQ(copy.getSigma(), original.getSigma());
    EXPECT_DOUBLE_EQ(copy.getRates().at(0), original.getRates().at(0));
    EXPECT_DOUBLE_EQ(copy.getRates().at(1), original.getRates().at(1));
    EXPECT_EQ(copy.getExerciseType(), original.getExerciseType());
}





