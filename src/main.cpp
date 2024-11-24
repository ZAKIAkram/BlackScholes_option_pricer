#include "BS_pricer.h"

#include <iostream>

int main() {

    double S = 100.0;   // Stock price
    double K = 100.0;   // Strike
    double T = 1.0;     // Maturity
    double r = 0.05;    // Risk free rate
    double sigma = 0.2; // Volatility

    // Call and Put price 
    double callPrice = blackScholesPrice('c', S, K, T, r, sigma);
    double putPrice = blackScholesPrice('p', S, K, T, r, sigma);

    // Results
    std::cout << "Call option's price: " << callPrice << std::endl;
    std::cout << "Put option's price: " << putPrice << std::endl;

    return 0;
}
