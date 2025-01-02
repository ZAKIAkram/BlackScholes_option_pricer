#pragma once

#include <vector>
#include <string>
#include "Option.hpp"

class BlackScholesSolver {
private:

    /**
     * Calculates the Black-Scholes price for a single set of parameters.
     * @param S Current stock price.
     * @param option Option object containing parameters.
     * @param timeToMaturity Time to maturity.
     * @return The calculated option price.
     */
    static double blackScholesPrice(double, const Option&, int, int);
public:
    /**
     * Static method to solve the Black-Scholes grid for the given option.
     * @param option The option for which to calculate prices.
     * @param N Number of time steps.
     * @param M Number of stock price steps.
     * @return A 2D grid of option prices.
     */
    static std::vector<std::vector<double>> solve(const Option& option, int N, int M);

};
