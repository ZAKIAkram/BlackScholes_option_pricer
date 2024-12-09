#include <iostream>
#include <cmath>
#include <memory>
#include "Option.hpp"
#include "EuropeanOption.hpp"
#include "AmericanOption.hpp"
#include "Payoff.hpp"
#include "CallPayoff.hpp"
#include "PutPayoff.hpp"
#include "CrankNicolsonSolver.hpp"

// Black-Scholes price calculation function
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

    double S = 100.0;       
    double K = 100.0;       
    double T = 1.0;         
    double r = 0.05;       
    double sigma = 0.2;
    int M = 100, N = 100;

    auto callPayoff = std::make_shared<CallPayoff>(K);
    auto putPayoff = std::make_shared<PutPayoff>(K);

    EuropeanOption callOption(callPayoff, T, sigma, r, r);
    EuropeanOption putOption(putPayoff, T, sigma, r, r);
    AmericanOption americanCallOption(callPayoff, T, sigma, r, r);
    AmericanOption americanPutOption(putPayoff, T, sigma, r, r);


    auto europeanCallGrid = CrankNicolsonSolver::solve(callOption, N, M);
    auto europeanPutGrid = CrankNicolsonSolver::solve(putOption, N, M);
    auto americanCallGrid = CrankNicolsonSolver::solve(americanCallOption, N, M);
    auto americanPutGrid = CrankNicolsonSolver::solve(americanPutOption, N, M);

    size_t mid = M / 2;
    double S_max = 2.0 * K; 
    double ds = S_max / (M);

    int S_index = static_cast<int>(S * M / (2 * K));

    double crankCallPrice = europeanCallGrid[0][S_index]; 
    double crankPutPrice = europeanPutGrid[0][S_index];   
    double crankAmericanCallPrice = americanCallGrid[0][S_index];
    double crankAmericanPutPrice = americanPutGrid[0][S_index];

    double bsCallPrice = blackScholesPrice(S, K, T, r, sigma, "Call");
    double bsPutPrice = blackScholesPrice(S, K, T, r, sigma, "Put");

    std::cout << "Black-Scholes Call Price: " << bsCallPrice << std::endl;
    std::cout << "Crank-Nicholson European Call Price: " << crankCallPrice << std::endl;
    std::cout << "Crank-Nicolson American Call Price: " << crankAmericanCallPrice << std::endl;
    std::cout << "Call Price Difference (European - American): " << std::abs(bsCallPrice - crankAmericanCallPrice) << std::endl;

    std::cout << "Black-Scholes Put Price: " << bsPutPrice << std::endl;
    std::cout << "Crank-Nicolson European Put Price: " << crankPutPrice << std::endl;
    std::cout << "Crank-Nicolson American Put Price: " << crankAmericanPutPrice << std::endl;
    std::cout << "Put Price Difference (European - American): " << std::abs(bsPutPrice - crankAmericanPutPrice) << std::endl;

    return 0;
}
