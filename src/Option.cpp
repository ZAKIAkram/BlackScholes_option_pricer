#include "Option.hpp"


Option::Option(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma, int N) : _maturity(maturity), _sigma(sigma), _rates(0., 0.), _optionPrices(N), _spot(N){
	_payoff = payoff;
}
Option::Option(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma, double rate_0, double rate_1, int N) : _maturity(maturity), _sigma(sigma), _rates(rate_0, rate_1) {
	_payoff = payoff;
}

Option::Option(const Option& other) : _maturity(other._maturity), _sigma(other._sigma), _rates(other._rates),  _optionPrices(other._optionPrices), _spot(other._spot){
	_payoff = other._payoff;
}

//VanillaOption::~VanillaOption() {
//	delete _payoff;
//}

double Option::getMaturity() const {
	return _maturity;
}

Rates Option::getRates() const{
	return _rates;
}

double Option::getStrike() const {
	return _payoff->getStrike();
}

void Option::setMaturity(double maturity) {
	_maturity = maturity;
}

double Option::computePayoff(double spot) const{
	return (*_payoff)(spot);
}
std::string Option::getType() const {
	return _payoff->getType();
}

