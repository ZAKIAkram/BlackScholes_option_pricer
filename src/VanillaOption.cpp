#include "VanillaOption.hpp"


VanillaOption::VanillaOption(const Payoff& payoff, double maturity, double sigma, int N) : _maturity(maturity), _sigma(sigma), _rates(0., 0., N){
	_payoff = payoff.clone();
}
VanillaOption::VanillaOption(const Payoff& payoff, double maturity, double sigma, double rate_0, double rate_1, int N) : _maturity(maturity), _sigma(sigma), _rates(rate_0, rate_1, N) {
	_payoff = payoff.clone();

}

VanillaOption::VanillaOption(const VanillaOption& other) {
	_maturity = other.getMaturity();
	_payoff = other._payoff->clone();
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

