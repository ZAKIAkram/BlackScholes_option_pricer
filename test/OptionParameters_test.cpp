#include <gtest/gtest.h>
#include "OptionParameters.hpp"
#include "Rates.hpp"  

TEST(OptionParametersTest, ConstructorTest) {
    // Set up the Rates object for testing
    Rates riskFreeRates(0.03, 0.05);  // 3% at t=0, 5% at t=1

    // Define option parameters for testing
    OptionParameters params = {
        OptionParameters::CALL,                // Put option
        OptionParameters::EUROPEAN,           // European exercise
        1.0,                              // Maturity T (1 year)
        100.0,                            // Strike price
        0.0,                              // Computation date T0 (start at time 0)
        {0.0, 0.25, 0.5, 0.75, 1.0},      // Time mesh (in years)
        {50, 75, 100, 125, 150},          // Spot mesh (example, not used in price calculation)
        100.0,                            // Current spot price S0
        riskFreeRates,                    // Risk-free rate object
        0.2,                               // Volatility (20%)
        0.0                              //dividend  

    };

    // Validate that the constructor properly initialized the member variables using getter functions
    EXPECT_EQ(params.getContractType(), OptionParameters::CALL);
    EXPECT_EQ(params.getExerciseType(), OptionParameters::EUROPEAN);
    EXPECT_DOUBLE_EQ(params.getMaturity(), 1.0);
    EXPECT_DOUBLE_EQ(params.getStrike(), 100.0);
    EXPECT_DOUBLE_EQ(params.getComputationDate(), 0.0);
    EXPECT_EQ(params.getTimeMesh().size(), 5); // Should match the number of elements in the time mesh
    EXPECT_EQ(params.getSpotMesh().size(), 5); // Should match the number of elements in the spot mesh
    EXPECT_DOUBLE_EQ(params.getSpotPrice(), 100.0);
    EXPECT_DOUBLE_EQ(params.getVolatility(), 0.2);
    EXPECT_DOUBLE_EQ(params.getDividend(), 0.0);
}

TEST(OptionParametersTest, GetterFunctionsTest) {
    // Set up the Rates object for testing
    Rates riskFreeRates(0.03, 0.05);

    // Define option parameters for testing
    OptionParameters params(
        OptionParameters::PUT,                // Put option
        OptionParameters::AMERICAN,           // American exercise
        0.5,                                  // Maturity T (0.5 years)
        120.0,                                // Strike price
        0.1,                                  // Computation date T0
        { 0.0, 0.25, 0.5 },                     // Time mesh
        { 60, 80, 100 },                        // Spot mesh
        110.0,                                // Spot price S0
        riskFreeRates,                        // Risk-free rate object
        0.25,                                 // Volatility
        0.05                                  // Dividend
    );

    // Validate the getter functions to ensure correct values
    EXPECT_EQ(params.getContractType(), OptionParameters::PUT);
    EXPECT_EQ(params.getExerciseType(), OptionParameters::AMERICAN);
    EXPECT_DOUBLE_EQ(params.getMaturity(), 0.5);
    EXPECT_DOUBLE_EQ(params.getStrike(), 120.0);
    EXPECT_DOUBLE_EQ(params.getComputationDate(), 0.1);
    EXPECT_EQ(params.getTimeMesh().size(), 3); // 3 values in the time mesh
    EXPECT_EQ(params.getSpotMesh().size(), 3); // 3 values in the spot mesh
    EXPECT_DOUBLE_EQ(params.getSpotPrice(), 110.0);
    EXPECT_DOUBLE_EQ(params.getVolatility(), 0.25);
    EXPECT_DOUBLE_EQ(params.getDividend(), 0.05);
}
