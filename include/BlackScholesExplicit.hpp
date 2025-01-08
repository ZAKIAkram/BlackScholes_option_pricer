#pragma once

#include <vector>
#include <string>
#include "Option.hpp"

class BlackScholesExplicitSolver {

public:
    /**
     * Calculates the Black-Scholes price for a single set of parameters.
     * @param S Current stock price.
     * @param option Option object containing parameters.
     * @return The calculated option price at t=0
     */
    static double blackScholesPrice(double S, const Option& option);

};
