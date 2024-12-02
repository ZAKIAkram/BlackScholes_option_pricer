#include "gtest/gtest.h"
#include "OptionParameters.hpp"
#include "Rates.hpp"
#include "BS_pricer.hpp" 

// Function to compute Black-Scholes price for testing
double blackScholesPrice(const OptionParameters& params);

TEST(BlackScholesTests, TestEuropeanCallOption) {
    // Create risk-free rate object (linear interpolation between 3% at t=0 and 5% at t=1)
    Rates riskFreeRates(0.03, 0.05);

    // Define option parameters for a Call option
    OptionParameters params = {
        OptionParameters::CALL,               // Call option
        OptionParameters::EUROPEAN,           // European exercise
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
    EXPECT_NEAR(priceAtT0, 10.4506, 0.01);  

    // Test Black-Scholes pricing for a Call option at a later time (e.g., t = 0.25)
    OptionParameters paramsAtT1 = params;  // Make a copy of params
    paramsAtT1.setComputationDate(0.25);   // Set computation time to 0.25 years

    double priceAtT1 = blackScholesPrice(paramsAtT1);
    EXPECT_NEAR(priceAtT1, 9.8253, 0.01); 
}

TEST(BlackScholesTests, TestEuropeanPutOption) {
    // Create risk-free rate object (linear interpolation between 3% at t=0 and 5% at t=1)
    Rates riskFreeRates(0.03, 0.05);

    // Define option parameters for a Put option
    OptionParameters params = {
        OptionParameters::PUT,                // Put option
        OptionParameters::EUROPEAN,           // European exercise
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
    EXPECT_NEAR(priceAtT0, 10.4506, 0.01);  

    // Test Black-Scholes pricing for a Put option at a later time (e.g., t = 0.25)
    OptionParameters paramsAtT1 = params;  // Make a copy of params
    paramsAtT1.setComputationDate(0.25);   // Set computation time to 0.25 years

    double priceAtT1 = blackScholesPrice(paramsAtT1);
    EXPECT_NEAR(priceAtT1, 9.8253, 0.01);  
}
