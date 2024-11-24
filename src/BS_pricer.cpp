#include "BS_pricer.h"
#include <iostream>

//library
#include <cmath>

// CDF for normal distribution implemntation
double normalCDF(double x) {
    return 0.5 * erfc(-x * (1 / sqrt(2))); 
}

// Black-Scholes function
double blackScholesPrice(char optionType, double S, double K, double T, double r, double sigma) {
    double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);

    if (optionType == 'c') { // Call
        return S * normalCDF(d1) - K * exp(-r * T) * normalCDF(d2);
    }
    else if (optionType == 'p') { // Put
        return K * exp(-r * T) * normalCDF(-d2) - S * normalCDF(-d1);
    }
    else {
        std::cerr << "option type invalid. Type 'c' for Call or 'p' for Put." << std::endl;
        return -1.0;
    }
}
