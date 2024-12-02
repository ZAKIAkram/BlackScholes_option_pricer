#include <gtest/gtest.h>
#include "OptionParameters.hpp"
#include "Rates.hpp"

// Test Fixture for OptionParameters
class OptionParametersTest : public ::testing::Test {
protected:
    // Default option parameters for testing
    Rates riskFreeRate{ 0.05 };  // 5% risk-free rate
    std::vector<double> timeMesh{ 0.0, 0.5, 1.0 };  // Example time mesh: [0.0, 0.5, 1.0]
    std::vector<double> spotMesh{ 80.0, 100.0, 120.0 };  // Example spot mesh: [80, 100, 120]

    OptionParameters optionParams{
        OptionParameters::CALL,        // Contract type: CALL
        OptionParameters::EUROPEAN,    // Exercise type: EUROPEAN
        1.0,                           // Maturity: 1 year
        100.0,                         // Strike price: 100
        0.0,                           // Computation date: today
        timeMesh,                      // Time mesh
        spotMesh,                      // Spot mesh
        100.0,                         // Spot price: 100
        riskFreeRate,                  // Risk-free rate
        0.2,                           // Volatility: 20%
        0.0                            // Dividend: 0%
    };
};

// Test default constructor (using default parameters)
TEST_F(OptionParametersTest, DefaultConstructor) {
    OptionParameters defaultOptionParams;

    // Test default values
    EXPECT_EQ(defaultOptionParams.getContractType(), OptionParameters::CALL);
    EXPECT_EQ(defaultOptionParams.getExerciseType(), OptionParameters::EUROPEAN);
    EXPECT_DOUBLE_EQ(defaultOptionParams.getMaturity(), 1.0);
    EXPECT_DOUBLE_EQ(defaultOptionParams.getStrike(), 100.0);
    EXPECT_DOUBLE_EQ(defaultOptionParams.getComputationDate(), 0.0);
    EXPECT_DOUBLE_EQ(defaultOptionParams.getSpotPrice(), 100.0);
    EXPECT_DOUBLE_EQ(defaultOptionParams.getVolatility(), 0.2);
    EXPECT_DOUBLE_EQ(defaultOptionParams.getDividend(), 0.0);
    EXPECT_EQ(defaultOptionParams.getTimeMesh().size(), 0);  // Empty by default
    EXPECT_EQ(defaultOptionParams.getSpotMesh().size(), 0);  // Empty by default
}

// Test parameterized constructor
TEST_F(OptionParametersTest, ParameterizedConstructor) {
    // Check that the parameters set via the constructor are correctly set
    EXPECT_EQ(optionParams.getContractType(), OptionParameters::CALL);
    EXPECT_EQ(optionParams.getExerciseType(), OptionParameters::EUROPEAN);
    EXPECT_DOUBLE_EQ(optionParams.getMaturity(), 1.0);
    EXPECT_DOUBLE_EQ(optionParams.getStrike(), 100.0);
    EXPECT_DOUBLE_EQ(optionParams.getComputationDate(), 0.0);
    EXPECT_DOUBLE_EQ(optionParams.getSpotPrice(), 100.0);
    EXPECT_DOUBLE_EQ(optionParams.getVolatility(), 0.2);
    EXPECT_DOUBLE_EQ(optionParams.getDividend(), 0.0);

    // Check the size of the time and spot meshes
    EXPECT_EQ(optionParams.getTimeMesh().size(), 3);  // Expected size of 3
    EXPECT_EQ(optionParams.getSpotMesh().size(), 3);  // Expected size of 3

    // Check that the time and spot mesh values are correct
    EXPECT_DOUBLE_EQ(optionParams.getTimeMesh()[0], 0.0);
    EXPECT_DOUBLE_EQ(optionParams.getTimeMesh()[1], 0.5);
    EXPECT_DOUBLE_EQ(optionParams.getTimeMesh()[2], 1.0);

    EXPECT_DOUBLE_EQ(optionParams.getSpotMesh()[0], 80.0);
    EXPECT_DOUBLE_EQ(optionParams.getSpotMesh()[1], 100.0);
    EXPECT_DOUBLE_EQ(optionParams.getSpotMesh()[2], 120.0);
}

// Test Risk-Free Rate
TEST_F(OptionParametersTest, RiskFreeRate) {
    EXPECT_DOUBLE_EQ(optionParams.getRiskFreeRate().at(0), 0.05);  // Should return 0.05 for t = 0
}

// Test that invalid times throw an exception
TEST_F(OptionParametersTest, InvalidRiskFreeRateTime) {
    EXPECT_THROW(optionParams.getRiskFreeRate().at(2.0), std::out_of_range);  // Should throw an exception
}

