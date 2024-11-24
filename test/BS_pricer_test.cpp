#include <gtest/gtest.h>
#include "BS_pricer.h"

//TEST(AnotherTest, TrueIsTrue) {
 //   EXPECT_TRUE(true);
//}


// Tolerance for comparing floating-point numbers
const double TOLERANCE = 1e-6;

// Test Case: Verify call option pricing
TEST(BlackScholesTest, CallOptionPrice) {
    double S = 100.0;   // Spot price
    double K = 100.0;   // Strike price
    double T = 1.0;     // Time to maturity (in years)
    double r = 0.05;    // Risk-free rate
    double sigma = 0.2; // Volatility

    double expectedCallPrice = 10.4506; // Pre-computed expected result
    double computedCallPrice = blackScholesPrice('c', S, K, T, r, sigma);

    EXPECT_NEAR(computedCallPrice, expectedCallPrice, TOLERANCE);
}

// Test Case: Verify put option pricing
TEST(BlackScholesTest, PutOptionPrice) {
    double S = 100.0;   // Spot price
    double K = 100.0;   // Strike price
    double T = 1.0;     // Time to maturity (in years)
    double r = 0.05;    // Risk-free rate
    double sigma = 0.2; // Volatility

    double expectedPutPrice = 5.5735; // Pre-computed expected result
    double computedPutPrice = blackScholesPrice('p', S, K, T, r, sigma);

    EXPECT_NEAR(computedPutPrice, expectedPutPrice, TOLERANCE);
}

// Test Case: Verify invalid option type handling
TEST(BlackScholesTest, InvalidOptionType) {
    double S = 100.0;
    double K = 100.0;
    double T = 1.0;
    double r = 0.05;
    double sigma = 0.2;

    double invalidResult = blackScholesPrice('x', S, K, T, r, sigma);

    EXPECT_EQ(invalidResult, -1.0); // Expect -1.0 as defined in the implementation
}


