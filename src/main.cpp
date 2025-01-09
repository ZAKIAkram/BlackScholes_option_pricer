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
#include "Greeks.hpp"
#include "BlackScholesExplicit.hpp"

int main() {

    double S = 100.0;       
    double K = 100.0;       
    double T = 1.0;         
    double r = 0.05;       
    double sigma = 0.25;
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

    Greeks europeanCallGreeks(callOption, europeanCallGrid, N, M);
    Greeks europeanPutGreeks(putOption, europeanPutGrid, N, M);
    Greeks americanCallGreeks(americanCallOption, americanCallGrid, N, M);
    Greeks americanPutGreeks(americanPutOption, americanPutGrid, N, M);

    int mid = M / 2;
    double S_max = 2.0 * K; 
    double ds = S_max / (M);

    int S_index = static_cast<int>(S * M / (2 * K));

    double crankCallPrice = europeanCallGrid[S_index][0];
    double crankPutPrice = europeanPutGrid[S_index][0];
    double crankAmericanCallPrice = americanCallGrid[S_index][0];
    double crankAmericanPutPrice = americanPutGrid[S_index][0];

    double bsCallPrice = BlackScholesExplicitSolver::blackScholesPrice(S, callOption);
    double bsPutPrice = BlackScholesExplicitSolver::blackScholesPrice(S, putOption);

    std::cout << "Black-Scholes Call Price: " << bsCallPrice << std::endl;
    std::cout << "Crank-Nicholson European Call Price: " << crankCallPrice << std::endl;
    std::cout << "Crank-Nicolson American Call Price: " << crankAmericanCallPrice << std::endl;
    std::cout << "Call Price Difference (European - American): " << std::abs(bsCallPrice - crankAmericanCallPrice) << std::endl;

    std::cout << "Black-Scholes Put Price: " << bsPutPrice << std::endl;
    std::cout << "Crank-Nicolson European Put Price: " << crankPutPrice << std::endl;
    std::cout << "Crank-Nicolson American Put Price: " << crankAmericanPutPrice << std::endl;

    return 0;
}


