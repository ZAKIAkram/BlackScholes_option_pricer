#include "BS_pricer.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

// Helper function: CDF of standard normal distribution
double normalCDF(double x) {
    return 0.5 * std::erfc(-x * (1 / sqrt(2)));
}

// Black-Scholes formula for European options
double blackScholesFormula(double S, double K, double T, double t, double r, double sigma, double y, ContractType type) {
    if (T < t) {
        throw std::invalid_argument("Maturity T must be greater than the current time t.");
    }

    double tau = T - t;   // Time to maturity
    double d1 = (std::log(S / K) + (r - y + 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
    double d2 = d1 - sigma * std::sqrt(tau);

    if (type == ContractType::Call) {
        return S * std::exp(-y * tau) * normalCDF(d1) - K * std::exp(-r * tau) * normalCDF(d2);
    }
    else {   // Put option
        return K * std::exp(-r * tau) * normalCDF(-d2) - S * std::exp(-y * tau) * normalCDF(-d1);
    }
}

// Compute Black-Scholes price for all times in the time mesh
double blackScholesPrice(const OptionParameters& params) {
    if (params.exerciseType != ExerciseType::European) {
        throw std::invalid_argument("Black-Scholes formula is only valid for European options.");
    }

    // Interpolate rates for the computation date
    double T0 = params.computationDate;
    double T = params.maturity;
    std::vector<double> rates;
    for (double t : params.timeMesh) {
        double t_normalized = (t - T0) / (T - T0);   // Normalize time to [0, 1]
        rates.push_back(params.riskFreeRate.at(t_normalized));
    }

    // Compute prices for each time in the time mesh
    for (size_t i = 0; i < params.timeMesh.size(); ++i) {
        double t = params.timeMesh[i];
        double rate = rates[i];

        // Use Black-Scholes formula
        double price = blackScholesFormula(
            params.spotPrice,
            params.strike,
            params.maturity,
            t,
            rate,
            params.volatility,
            params.dividend,
            params.contractType
        );

        // Output the time in months
        std::cout << "Time: " << (t * 12) << " months, Price: " << price << std::endl;
    }

    // Return the price for the computation date (t = T0)
    double rateAtT0 = params.riskFreeRate.at(0.0);   // Rate at the start

    return blackScholesFormula(
        params.spotPrice,
        params.strike,
        params.maturity,
        T0,
        rateAtT0,
        params.volatility,
        params.dividend,
        params.contractType
    );
}


