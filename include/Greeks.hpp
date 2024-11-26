#pragma once

#include "BS_pricer.hpp"


struct Greeks {
    double delta;  // Sensitivity to changes in spot price
    double gamma;  // Sensitivity to changes in delta (2nd derivative wrt spot)
    double theta;  // Sensitivity to the passage of time
    double rho;    // Sensitivity to changes in interest rate
    double vega;   // Sensitivity to changes in volatility
};

// Function to calculate Black-Scholes price
std::vector<Greeks> calculateGreeks(const OptionParameters& params);
