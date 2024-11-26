#include <iostream>
#include <vector>
#include <cmath>
#include "Greeks.hpp"
#include "BS_pricer.hpp"
#include "Rates.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Cumulative normal distribution function
double normCDF(double x) {
    return 0.5 * erfc(-x / std::sqrt(2));
}

// Probability density function of the standard normal distribution
double normPDF(double x) {
    return std::exp(-0.5 * x * x) / std::sqrt(2 * M_PI);
}

// Function to calculate Black-Scholes Greeks for all times in the time mesh
std::vector<Greeks> calculateGreeks(const OptionParameters& params) {
    if (params.exerciseType != ExerciseType::European) {
        throw std::invalid_argument("Black-Scholes formula is only valid for European options.");
    }

    double S = params.spotPrice;
    double K = params.strike;
    double T = params.maturity;
    double sigma = params.volatility;
    double y = params.dividend;


    std::vector<Greeks> allGreeks;

    // Iterate over the time mesh
    for (double t : params.timeMesh) {
        if (T <= t) {
            throw std::invalid_argument("Maturity T must be greater than the computation time t.");
        }
        double T0 = params.computationDate;
        double T = params.maturity;
         double t_normalized = (t - T0) / (T - T0);   // Normalize time to [0, 1]
        // Compute risk-free rate at t
        double r = params.riskFreeRate.at(t_normalized);

        double tau = T - t; // Time to maturity
        double d1 = (std::log(S / K) + (r - y + 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
        double d2 = d1 - sigma * std::sqrt(tau);

        Greeks greeks;

        // Delta
        if (params.contractType == ContractType::Call) {
            greeks.delta = std::exp(-y * tau) * normCDF(d1);
        }
        else {
            greeks.delta = std::exp(-y * tau) *(normCDF(d1) - 1);
        }

        // Gamma
        greeks.gamma = (normPDF(d1) * std::exp(-y * tau)) / (S * sigma * std::sqrt(tau));

        // Theta
        if (params.contractType == ContractType::Call) {
            greeks.theta = -(S * normPDF(d1) * sigma * std::exp(-y * tau)) / (2 * std::sqrt(tau)) +
                y * S * std::exp(-y * tau) * normCDF(d1) -
                r * K * std::exp(-r * tau) * normCDF(d2);
        }
        else {
            greeks.theta = -(S * normPDF(d1) * sigma * std::exp(-y * tau)) / (2 * std::sqrt(tau)) -
                y * S * std::exp(-y * tau) * normCDF(-d1) +
                r * K * std::exp(-r * tau) * normCDF(-d2);
        }

        // Rho
        if (params.contractType == ContractType::Call) {
            greeks.rho = K * tau * std::exp(-r * tau) * normCDF(d2);
        }
        else {
            greeks.rho = -K * tau * std::exp(-r * tau) * normCDF(-d2);
        }

        // Vega
        greeks.vega = S * std::exp(-y * tau) * std::sqrt(tau) * normPDF(d1);

        // Store the Greeks for this time step
        allGreeks.push_back(greeks);

        // Output the results
        std::cout << "Time: " << (t * 12) << " months" << std::endl;
        std::cout << "  Delta: " << greeks.delta << std::endl;
        std::cout << "  Gamma: " << greeks.gamma << std::endl;
        std::cout << "  Theta: " << greeks.theta << std::endl;
        std::cout << "  Rho:   " << greeks.rho << std::endl;
        std::cout << "  Vega:  " << greeks.vega << std::endl;
    }

    return allGreeks;
}
