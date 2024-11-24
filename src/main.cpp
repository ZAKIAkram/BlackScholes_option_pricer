//#include "BS_pricer.h"
//
//#include <iostream>
//
////int main() {
////
////    double S = 100.0;   // Stock price
////    double K = 100.0;   // Strike
////    double T = 1.0;     // Maturity
////    double r = 0.05;    // Risk free rate
////    double sigma = 0.2; // Volatility
////
////    // Call and Put price 
////    double callPrice = blackScholesPrice('c', S, K, T, r, sigma);
////    double putPrice = blackScholesPrice('p', S, K, T, r, sigma);
////
////    // Results
////    std::cout << "Call option's price: " << callPrice << std::endl;
////    std::cout << "Put option's price: " << putPrice << std::endl;
////
////    return 0;
////}
////
//
#include <iostream>
#include "BS_pricer_update.hpp"

int main() {
    try {
        // Create a risk-free rate object
        Rates riskFreeRate(0.01, 0.05, 10); 

        // Define option parameters
        OptionParameters option{
            ContractType::Call,                         // Call option
            ExerciseType::European,                     // European exercise
            0.5,                                        // Maturity T = 1 year
            100.0,                                      // Strike price
            0.0,                                        // Computation date T0 = today
            {0.0, 0.5, 1.0},                            // Time mesh
            {80.0, 100.0, 120.0},                       // Spot mesh
            100.0,                                      // Spot price S0
            riskFreeRate,                               // Risk-free rate object
            0.2                                         // Volatility
        };

        // Compute the Black-Scholes price
        double price = blackScholesPrice(option);

        // Output the result
        std::cout << "Black-Scholes Price: " << price << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}




