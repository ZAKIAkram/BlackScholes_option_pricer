#include "EuropeanOption.hpp"


EuropeanOption::EuropeanOption(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma, double rate_0, double rate_1) : Option(payoff, maturity, sigma, rate_0, rate_1) {}

EuropeanOption::EuropeanOption(const EuropeanOption& other) : Option(other._payoff, other._maturity, other._sigma, other._rates.at(0), other._rates.at(1)) {}
std::string EuropeanOption::getExerciseType() const{
	return "European";
}

std::shared_ptr<Option> EuropeanOption::clone() const {
	return std::make_shared<EuropeanOption>(*this);
}
