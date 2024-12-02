#include "gtest/gtest.h"
#include "OptionParameters.hpp"
#include "Rates.hpp"
#include "Greeks.hpp" 
#include "BS_pricer.hpp"

// Function to calculate Black-Scholes Greeks for testing
std::vector<Greeks> calculateGreeks(const OptionParameters& params);

TEST(BlackScholesGreeksTests, TestEuropeanCallOptionGreeks) {
    // Create risk-free rate object (linear interpolation between 3% at t=0 and 5% at t=1)
    Rates riskFreeRates(0.03, 0.05);

    // Define option parameters for a Call option
    OptionParameters params = {
        OptionParameters::CALL;              // Call option
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
    // Calculate Greeks for the Call option
    std::vector<Greeks> greeks = calculateGreeks(params);

    // Test the Greeks at different times in the time mesh
    for (size_t i = 0; i < greeks.size(); ++i) {
        const Greeks& greek = greeks[i];

        // Check the Delta for the Call option (Should be positive and close to 1 at t=0)
        EXPECT_GT(greek.delta, 0);
        EXPECT_LT(greek.delta, 1);

        // Check Gamma: It should be positive
        EXPECT_GT(greek.gamma, 0);

        // Check Theta: It should be negative for a Call option
        EXPECT_LT(greek.theta, 0);

        // Check Rho: Positive for a Call option
        EXPECT_GT(greek.rho, 0);

        // Check Vega: Positive for a Call option
        EXPECT_GT(greek.vega, 0);

        // Output the values for debugging (Optional)
        std::cout << "Time: " << (params.getTimeMesh()[i] * 12) << " months" << std::endl;
        std::cout << "  Delta: " << greek.delta << std::endl;
        std::cout << "  Gamma: " << greek.gamma << std::endl;
        std::cout << "  Theta: " << greek.theta << std::endl;
        std::cout << "  Rho:   " << greek.rho << std::endl;
        std::cout << "  Vega:  " << greek.vega << std::endl;
    }
}

TEST(BlackScholesGreeksTests, TestEuropeanPutOptionGreeks) {
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

    // Calculate Greeks for the Put option
    std::vector<Greeks> greeks = calculateGreeks(params);

    // Test the Greeks at different times in the time mesh
    for (size_t i = 0; i < greeks.size(); ++i) {
        const Greeks& greek = greeks[i];

        // Check the Delta for the Put option (Should be negative and close to -1 at t=0)
        EXPECT_LT(greek.delta, 0);
        EXPECT_GT(greek.delta, -1);

        // Check Gamma: It should be positive
        EXPECT_GT(greek.gamma, 0);

        // Check Theta: It should be negative for a Put option
        EXPECT_LT(greek.theta, 0);

        // Check Rho: Negative for a Put option
        EXPECT_LT(greek.rho, 0);

        // Check Vega: Positive for a Put option
        EXPECT_GT(greek.vega, 0);

        // Output the values for debugging (Optional)
        std::cout << "Time: " << (params.getTimeMesh()[i] * 12) << " months" << std::endl;
        std::cout << "  Delta: " << greek.delta << std::endl;
        std::cout << "  Gamma: " << greek.gamma << std::endl;
        std::cout << "  Theta: " << greek.theta << std::endl;
        std::cout << "  Rho:   " << greek.rho << std::endl;
        std::cout << "  Vega:  " << greek.vega << std::endl;
    }
}
