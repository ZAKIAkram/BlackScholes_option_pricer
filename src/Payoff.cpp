#include "Payoff.hpp"


Payoff::Payoff(double strike) : _strike(strike) {}

double Payoff::getStrike() const {
	return _strike;
}

