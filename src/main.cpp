//#include <iostream>
//#include <cmath>
//#include <memory>
//#include "Option.hpp"
//#include "EuropeanOption.hpp"
//#include "AmericanOption.hpp"
//#include "Payoff.hpp"
//#include "CallPayoff.hpp"
//#include "PutPayoff.hpp"
//#include "CrankNicolsonSolver.hpp"
//#include "Greeks.hpp"
//
//double blackScholesPrice(double S, double K, double T, double r, double sigma, const std::string& optionType) {
//    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
//    double d2 = d1 - sigma * std::sqrt(T);
//
//    double N_d1 = 0.5 * (1.0 + std::erf(d1 / std::sqrt(2.0)));
//    double N_d2 = 0.5 * (1.0 + std::erf(d2 / std::sqrt(2.0)));
//    double N_minus_d1 = 1.0 - N_d1;
//    double N_minus_d2 = 1.0 - N_d2;
//
//    if (optionType == "Call") {
//        return S * N_d1 - K * std::exp(-r * T) * N_d2;
//    }
//    else if (optionType == "Put") {
//        return K * std::exp(-r * T) * N_minus_d2 - S * N_minus_d1;
//    }
//}
//
//int main() {
//
//    double S = 100.0;       
//    double K = 100.0;       
//    double T = 1.0;         
//    double r = 0.05;       
//    double sigma = 0.2;
//    int M = 100, N = 100;
//
//    auto callPayoff = std::make_shared<CallPayoff>(K);
//    auto putPayoff = std::make_shared<PutPayoff>(K);
//
//    EuropeanOption callOption(callPayoff, T, sigma, r, r);
//    EuropeanOption putOption(putPayoff, T, sigma, r, r);
//    AmericanOption americanCallOption(callPayoff, T, sigma, r, r);
//    AmericanOption americanPutOption(putPayoff, T, sigma, r, r);
//
//
//    auto europeanCallGrid = CrankNicolsonSolver::solve(callOption, N, M);
//    auto europeanPutGrid = CrankNicolsonSolver::solve(putOption, N, M);
//    auto americanCallGrid = CrankNicolsonSolver::solve(americanCallOption, N, M);
//    auto americanPutGrid = CrankNicolsonSolver::solve(americanPutOption, N, M);
//
//    Greeks europeanCallGreeks(callOption, europeanCallGrid, N, M);
//    Greeks europeanPutGreeks(putOption, europeanPutGrid, N, M);
//    Greeks americanCallGreeks(americanCallOption, americanCallGrid, N, M);
//    Greeks americanPutGreeks(americanPutOption, americanPutGrid, N, M);
//
//    size_t mid = M / 2;
//    double S_max = 2.0 * K; 
//    double ds = S_max / (M);
//
//    int S_index = static_cast<int>(S * M / (2 * K));
//
//    double crankCallPrice = europeanCallGrid[S_index][0];
//    double crankPutPrice = europeanPutGrid[S_index][0];
//    double crankAmericanCallPrice = americanCallGrid[S_index][0];
//    double crankAmericanPutPrice = americanPutGrid[S_index][0];
//
//    double bsCallPrice = blackScholesPrice(S, K, T, r, sigma, "Call");
//    double bsPutPrice = blackScholesPrice(S, K, T, r, sigma, "Put");
//
//    std::cout << "Black-Scholes Call Price: " << bsCallPrice << std::endl;
//    std::cout << "Crank-Nicholson European Call Price: " << crankCallPrice << std::endl;
//    std::cout << "Crank-Nicolson American Call Price: " << crankAmericanCallPrice << std::endl;
//    std::cout << "Call Price Difference (European - American): " << std::abs(bsCallPrice - crankAmericanCallPrice) << std::endl;
//
//    std::cout << "Black-Scholes Put Price: " << bsPutPrice << std::endl;
//    std::cout << "Crank-Nicolson European Put Price: " << crankPutPrice << std::endl;
//    std::cout << "Crank-Nicolson American Put Price: " << crankAmericanPutPrice << std::endl;
//    std::cout << "Put Price Difference (European - American): " << std::abs(bsPutPrice - crankAmericanPutPrice) << std::endl;
//
//    return 0;
//}




#include <iostream>
#include <Windows.h>

typedef void (*ComputeCrankNicolsonFunc)(int, int, double, double, double, double, double, double, const char*, const char*, double**, double*, double*, double*, double*, double**);

int main() {
    // Parameters
    int N = 100, M = 100;
    double S = 100.0, T = 1.0, K = 100.0, sigma = 0.2, r_0 = 0.05, r_1 = 0.05;
    const char* optionType = "call";
    const char* exerciceType = "European";

    // Allocate memory for outputs
    double** grid = new double* [M];  // 2D grid: M rows and N columns
    for (int i = 0; i < M; ++i) {
        grid[i] = new double[N];  // Allocate N columns for each row
    }

    double* delta = new double[M];  // 1D array of size M
    double* gamma = new double[M];  // 1D array of size M
    double* rho = new double[M];    // 1D array of size M
    double* vega = new double[M];   // 1D array of size M
    double** theta = new double* [M];  // 2D array: M rows for theta
    for (int i = 0; i < M; ++i) {
        theta[i] = new double[N];  // Allocate N columns for each row
    }

    // Load the DLL
    HMODULE hDLL = LoadLibrary("C:\\Users\\akram\\Desktop\\etudes_paris-saclay\\cpp_project\\cmake_project\\BS_pricer\\BS_pricer\\out\\build\\x64-Debug\\bin\\CN_pricer_lib.dll");
    if (!hDLL) {
        std::cerr << "Failed to load DLL" << std::endl;
        return 1;
    }

    // Get the function pointer
    ComputeCrankNicolsonFunc computeCrankNicolson =
        (ComputeCrankNicolsonFunc)GetProcAddress(hDLL, "computeCrankNicolson");

    if (!computeCrankNicolson) {
        std::cerr << "Failed to get function address" << std::endl;
        FreeLibrary(hDLL);
        return 1;
    }

    // Call the function
    computeCrankNicolson(N, M, S, T, K, sigma, r_0, r_1, optionType, exerciceType, grid, delta, gamma, rho, vega, theta);

    std::cout << "Computation done!" << std::endl;

    // Print grid as an example of output (optional)
    //std::cout << "Grid[0][0] = " << grid[0][0] << std::endl;

    // Free the memory
    for (int i = 0; i < M; ++i) {
        delete[] grid[i];  // Free each row of the grid
        delete[] theta[i]; // Free each row of theta
    }
    delete[] grid;      // Free the 2D grid pointer
    delete[] theta;     // Free the 2D theta pointer
    delete[] delta;     // Free delta
    delete[] gamma;     // Free gamma
    delete[] rho;       // Free rho
    delete[] vega;      // Free vega

    // Free the DLL
    FreeLibrary(hDLL);

    return 0;
}

