#include "Payoff.hpp"
//test

//Payoff::Payoff(double strike) : _strike(strike) {}

double payoff(double S, double K) {
    return std::max(S - K, 0.0); // European Call payoff
}
