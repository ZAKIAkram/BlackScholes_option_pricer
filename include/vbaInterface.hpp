#pragma once

#include "Option.hpp"
#include "EuropeanOption.hpp"
#include "AmericanOption.hpp"
#include "CallPayoff.hpp"
#include "PutPayoff.hpp"
#include "CrankNicolsonSolver.hpp"
#include "Greeks.hpp"
#include <memory>
#include <cstring>

extern "C" {
    __declspec(dllexport) void computeCrankNicolson(int N, int M, double S, double T, double K, double sigma, double r_0, double r_1, 
                                                    const char* optionType, const char* exerciceType, double** grid, double* delta, double* gamma, double* rho, double* vega, double** theta);
}

