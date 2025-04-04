#include "BlackScholesExplicit.hpp"

double BlackScholesExplicitSolver::blackScholesPrice(double S, const Option& option) {

    double T = option.getMaturity();
    double K = option.getStrike();
    double sigma = option.getSigma();
    Rates rates = option.getRates();
    double r = option.getRates().at(0); 
    std::string optionType = option.getType();


    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);

    double N_d1 = 0.5 * (1.0 + std::erf(d1 / std::sqrt(2.0)));
    double N_d2 = 0.5 * (1.0 + std::erf(d2 / std::sqrt(2.0)));
    double N_minus_d1 = 1.0 - N_d1;
    double N_minus_d2 = 1.0 - N_d2;

    if (optionType == "call") {
        return S * N_d1 - K * std::exp(-r * T) * N_d2;
    }
     return K * std::exp(-r * T) * N_minus_d2 - S * N_minus_d1;


}

