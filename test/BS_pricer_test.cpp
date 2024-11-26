#include "gtest/gtest.h"
#include "OptionParameters.hpp"
#include "Rates.hpp"

// Function to compute Black-Scholes price for testing
double blackScholesFormula(double S, double K, double T, double t, double r, double sigma, ContractType type);

TEST(BlackScholesTests, TestEuropeanCallOption) {
    // Create risk-free rate object (linear interpolation between 3% at t=0 and 5% at t=1)
    Rates riskFreeRates(0.03, 0.05);

    // Define option parameters
    OptionParameters params = {
        ContractType::Call,               // Call option
        ExerciseType::European,           // European exercise
        1.0,                              // Maturity T (1 year)
        100.0,                            // Strike price
        0.0,                              // Computation date T0 (start at time 0)
        {0.0, 0.25, 0.5, 0.75, 1.0},      // Time mesh (in years)
        {50, 75, 100, 125, 150},          // Spot mesh (example, not used in price calculation)
        100.0,                            // Current spot price S0
        riskFreeRates,                    // Risk-free rate object
        0.2                               // Volatility (20%)
    };

    // Test Black-Scholes pricing at computation date (T0)
    double priceAtT0 = blackScholesPrice(params);
    EXPECT_NEAR(priceAtT0, 10.4506, 0.01);  // Adjust expected value based on your calculation

    // Additional test: Test pricing at different times (e.g., time = 0.25)
    double priceAtT1 = blackScholesPrice(params);
    EXPECT_NEAR(priceAtT1, 9.8253, 0.01);  // Adjust expected value based on your calculation
}

TEST(BlackScholesTests, TestEuropeanPutOption) {
    // Create risk-free rate object (linear interpolation between 3% at t=0 and 5% at t=1)
    Rates riskFreeRates(0.03, 0.05);

    // Define option parameters for Put option
    OptionParameters params = {
        ContractType::Put,                // Put option
        ExerciseType::European,           // European exercise
        1.0,                              // Maturity T (1 year)
        100.0,                            // Strike price
        0.0,                              // Computation date T0 (start at time 0)
        {0.0, 0.25, 0.5, 0.75, 1.0},      // Time mesh (in years)
        {50, 75, 100, 125, 150},          // Spot mesh (example, not used in price calculation)
        100.0,                            // Current spot price S0
        riskFreeRates,                    // Risk-free rate object
        0.2                               // Volatility (20%)
    };

    // Test Black-Scholes pricing for a Put option
    double priceAtT0 = blackScholesPrice(params);
    EXPECT_NEAR(priceAtT0, 10.4506, 0.01);  // Adjust expected value based on your calculation

    // Additional test for Put option pricing at different times
    double priceAtT1 = blackScholesPrice(params);
    EXPECT_NEAR(priceAtT1, 9.8253, 0.01);  // Adjust expected value based on your calculation
}

