#include "PutPayoff.hpp"

PutPayoff::PutPayoff(double strike) : Payoff(strike) {}

double PutPayoff::operator()(double spot) const {
	return std::max(_strike - spot, 0.);
}

std::string PutPayoff::getType() const {
	return "put";
}

//Payoff* PutPayoff::clone() const {
//	return new PutPayoff(*this);
//}
