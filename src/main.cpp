#include <iostream>
#include <cmath>
#include <memory>
#include "Option.hpp"
#include "EuropeanOption.hpp"
#include "Payoff.hpp"
#include "CallPayoff.hpp"
#include "PutPayoff.hpp"
#include "CrankNicolsonSolver.hpp"

// Black-Scholes formula for European call and put options
double blackScholesPrice(double S, double K, double T, double r, double sigma, const std::string& optionType) {
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);

    double N_d1 = 0.5 * (1.0 + std::erf(d1 / std::sqrt(2.0)));
    double N_d2 = 0.5 * (1.0 + std::erf(d2 / std::sqrt(2.0)));
    double N_minus_d1 = 1.0 - N_d1;
    double N_minus_d2 = 1.0 - N_d2;

    if (optionType == "Call") {
        return S * N_d1 - K * std::exp(-r * T) * N_d2;
    }
    else if (optionType == "Put") {
        return K * std::exp(-r * T) * N_minus_d2 - S * N_minus_d1;
    }
    else {
        throw std::invalid_argument("Invalid option type. Use 'Call' or 'Put'.");
    }
}

int main() {
    // Define option parameters
    double S = 100.0;       // Spot price
    double K = 100.0;       // Strike price
    double T = 1.0;         // Time to maturity (1 year)
    double r = 0.05;        // Risk-free interest rate (5%)
    double sigma = 0.2;     // Volatility (20%)

    // Create payoff objects
    auto callPayoff = std::make_shared<CallPayoff>(K);
    auto putPayoff = std::make_shared<PutPayoff>(K);

    // Create options
    auto callOption = std::make_shared<EuropeanOption>(callPayoff, T, sigma, r, r, 100);
    auto putOption = std::make_shared<EuropeanOption>(putPayoff, T, sigma, r, r, 100);

    // Create Crank-Nicholson solver
    CrankNicolsonSolver solver(100, 100);

    // Solve for option prices using Crank-Nicholson
    auto callGrid = solver.solve(callOption);
    auto putGrid = solver.solve(putOption);

    // Extract the price at S = 100 (middle of the spot grid)
    size_t M = 100;
    size_t mid = M / 2; // Assuming spot grid is symmetric around S = 100
    double crankCallPrice = callGrid[50][1];
    double crankPutPrice = putGrid[50][1];

    // Compute Black-Scholes prices
    double bsCallPrice = blackScholesPrice(S, K, T, r, sigma, "Call");
    double bsPutPrice = blackScholesPrice(S, K, T, r, sigma, "Put");

    // Output results
    std::cout << "Black-Scholes Call Price: " << bsCallPrice << std::endl;
    std::cout << "Crank-Nicholson Call Price: " << crankCallPrice << std::endl;
    std::cout << "Call Price Difference: " << std::abs(bsCallPrice - crankCallPrice) << std::endl;

    std::cout << "Black-Scholes Put Price: " << bsPutPrice << std::endl;
    std::cout << "Crank-Nicholson Put Price: " << crankPutPrice << std::endl;
    std::cout << "Put Price Difference: " << std::abs(bsPutPrice - crankPutPrice) << std::endl;

    return 0;
}
