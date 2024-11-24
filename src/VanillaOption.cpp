#include "VanillaOption.hpp"


VanillaOption::VanillaOption(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma, int N) : _maturity(maturity), _sigma(sigma), _rates(0., 0.){
	_payoff = payoff;
}
VanillaOption::VanillaOption(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma, double rate_0, double rate_1, int N) : _maturity(maturity), _sigma(sigma), _rates(rate_0, rate_1) {
	_payoff = payoff;
}

VanillaOption::VanillaOption(const VanillaOption& other) : _maturity(other._maturity), _sigma(other._sigma), _rates(other._rates),  _optionPrices(other._optionPrices), _spot(other._spot){
	_payoff = other._payoff;
}

//VanillaOption::~VanillaOption() {
//	delete _payoff;
//}

double VanillaOption::getMaturity() const {
	return _maturity;
}

void VanillaOption::setMaturity(double maturity) {
	_maturity = maturity;
}

double VanillaOption::computePayoff(double spot) const{
	return (*_payoff)(spot);
}

