#include "CallPayoff.hpp"

CallPayoff::CallPayoff(double strike) : Payoff(strike) {}

double CallPayoff::operator()(double spot) const {
	return std::max(spot - _strike, 0.);
}

//Payoff* CallPayoff::clone() const {
//	return new CallPayoff(*this);
//}
