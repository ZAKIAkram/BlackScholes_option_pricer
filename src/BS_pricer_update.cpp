#include "BS_pricer_update.hpp"
#include <cmath>
#include <iostream>

// Black-Scholes price calculation
double blackScholesPrice(const OptionParameters& params) {
    if (params.exerciseType != ExerciseType::European) {
        std::cerr << "Only European options are supported in this implementation." << std::endl;
        return -1.0;
    }

    // Use the risk-free rate from the Rate class
    double r = params.riskFreeRate.at(params.maturity);
    double T = params.maturity - params.computationDate; // Time to maturity
    double S = params.spotPrice;                        // Spot price
    double K = params.strike;                           // Strike price
    double sigma = params.volatility;                   // Volatility

    // Black-Scholes formula calculations
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);

    // Call or Put
    if (params.contractType == ContractType::Call) {
        return S * 0.5 * erfc(-d1 * (1/sqrt(2))) - K * std::exp(-r * T) * 0.5 * erfc(-d2 * (1 / sqrt(2)));
    }
    else { // Put
        return K * std::exp(-r * T) * 0.5 * erfc(d2 * (1 / sqrt(2))) - S * 0.5 * erfc(d1 * (1 / sqrt(2)));
    }
}
