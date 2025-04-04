#include "Option.hpp"


Option::Option(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma) : _maturity(maturity), _sigma(sigma), _rates(0., 0.){
	_payoff = payoff;
}
Option::Option(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma, double rate_0, double rate_1) : _maturity(maturity), _sigma(sigma), _rates(rate_0, rate_1) {
	_payoff = payoff;
}

Option::Option(const Option& other) : _maturity(other._maturity), _sigma(other._sigma), _rates(other._rates){
	_payoff = other._payoff;
}

double Option::getMaturity() const {
	return _maturity;
}

Rates Option::getRates() const{
	return _rates;
}

double Option::getStrike() const {
	return _payoff->getStrike();
}

double Option::getSigma() const {
	return _sigma;
}

void Option::setMaturity(double maturity) {
	_maturity = maturity;
}
void Option::setSigma(double sigma) {
	_sigma = sigma;
}

void Option::setRates(Rates rates) {
	_rates = rates;
}

double Option::computePayoff(double spot) const{
	return (*_payoff)(spot);
}
std::string Option::getType() const {
	return _payoff->getType();
}

