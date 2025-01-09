/**
 * @file BlackScholesExplicit.hpp
 * @brief Header file for the BlackScholesExplicitSolver class, which calculates the explicit Black-Scholes price for options.
 */

#pragma once

#include <vector>
#include <string>
#include "Option.hpp"

 /**
  * @class BlackScholesExplicitSolver
  * @brief Class for calculating the Black-Scholes price for options.
  */

class BlackScholesExplicitSolver {
public:
    BlackScholesExplicitSolver() = delete;
    /**
     * Calculates the Black-Scholes price for a single set of parameters.
     * @param S Current spot price.
     * @param option Option object containing parameters.
     * @return The calculated option price at t=0
     */
    static double blackScholesPrice(double S, const Option& option);

};
