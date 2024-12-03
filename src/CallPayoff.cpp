#include "CallPayoff.hpp"

CallPayoff::CallPayoff(double strike) : Payoff(strike) {}

double CallPayoff::operator()(double spot) const {
	return std::max(spot - _strike, 0.);
}

std::string CallPayoff::getType() const {
	return "call";
}
//Payoff* CallPayoff::clone() const {
//	return new CallPayoff(*this);
//}
