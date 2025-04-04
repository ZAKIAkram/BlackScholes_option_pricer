/**
 * @file vbaInterface.hpp
 * @brief Header file for the VBA interface functions, which provide option pricing and Greeks calculation using the Crank-Nicolson method for our excel file
 */

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
#include <windows.h>
#include <oleauto.h>

/**
 * @brief Computes the Crank-Nicolson grid for option pricing and calculates the Greeks.
 *
 * @param N Number of time steps.
 * @param M Number of asset price steps.
 * @param S Spot.
 * @param T Maturity.
 * @param K Strike price.
 * @param sigma Volatility.
 * @param r_0 Initial interest rate.
 * @param r_1 Final interest rate.
 * @param optionType Type of the option ("call" or "put").
 * @param Exercisetype Exercise type of the option ("European" or "American").
 * @param outGrid Output SAFEARRAY for the computed grid (1d).
 * @param outDelta Output SAFEARRAY for the Delta values.
 * @param outGamma Output SAFEARRAY for the Gamma values.
 * @param outRho Output SAFEARRAY for the Rho values.
 * @param outVega Output SAFEARRAY for the Vega values.
 * @param outTheta Output SAFEARRAY for the Theta values (1d).
 */

extern "C" {
    __declspec(dllexport) void computeCrankNicolsonGrid(
        int N, int M, double S, double T, double K, double sigma,
        double r_0, double r_1,
        const char* optionType, const char* Exercisetype,
        SAFEARRAY** outGrid, SAFEARRAY** outDelta, SAFEARRAY** outGamma, SAFEARRAY** outRho, SAFEARRAY** outVega, SAFEARRAY** outTheta);
}

/**
 * @brief Copies data from a std::vector to a SAFEARRAY.
 *
 * @param data Input 1D vector containing the data to copy.
 * @param outArray Output SAFEARRAY.
 */
void copyArray(const std::vector<double>& data, SAFEARRAY** outArray);


/**
 * @brief Flattens a 2D std::vector and copies the data to a SAFEARRAY.
 *
 * @param data Input 2D vector containing the data to copy.
 * @param outArray Output SAFEARRAY.
 */
void flattenAndCopyArray(const std::vector<std::vector<double>>& data, SAFEARRAY** outArray);