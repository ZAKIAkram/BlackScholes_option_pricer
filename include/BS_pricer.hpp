#pragma once

#include <vector>
#include "Rates.hpp"
#include "OptionParameters.hpp"

//contract and exercise types
//enum class ContractType { Call, Put };
//enum class ExerciseType { European, American };

//option parameters
//struct OptionParameters {
//    ContractType contractType;    // Call or Put
//    ExerciseType exerciseType;    // European or American
//    double maturity;              // Option maturity T
//    double strike;                // Strike price
//    double computationDate;       // Computation date T0 (default is 0.0 for today)
//    std::vector<double> timeMesh; // Time mesh parameters
//    std::vector<double> spotMesh; // Spot mesh parameters
//    double spotPrice;             // Current spot price S0
//    Rates riskFreeRate;           // Risk-free rate class
//    double volatility;            // Volatility 
//    double dividend;              //dividend
//};

// Function to calculate Black-Scholes price
double blackScholesPrice(const OptionParameters& params);

