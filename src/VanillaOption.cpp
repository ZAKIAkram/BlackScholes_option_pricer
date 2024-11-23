#include "VanillaOption.hpp"


VanillaOption::VanillaOption(const Payoff& payoff, double maturity, double sigma) : _maturity(maturity), _sigma(sigma){
	_payoff = payoff.clone();
}

VanillaOption::VanillaOption(const VanillaOption& other) {
	_maturity = other.getMaturity();
	_payoff = other._payoff->clone();
}

VanillaOption::~VanillaOption() {
	delete _payoff;
}

double VanillaOption::getMaturity() const {
	return _maturity;
}

void VanillaOption::setMaturity(double maturity) {
	_maturity = maturity;
}

double VanillaOption::computePayoff(double spot) const{
	return (*_payoff)(spot);
}

