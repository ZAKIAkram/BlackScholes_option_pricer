#include "Americanoption.hpp"

AmericanOption::AmericanOption(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma, double rate_0, double rate_1) : Option(payoff, maturity, sigma, rate_0, rate_1) {}

AmericanOption::AmericanOption(const AmericanOption& other) : Option(other._payoff, other._maturity, other._sigma, other._rates.at(0), other._rates.at(1)){}

std::string AmericanOption::getExerciseType() const {
	return "American";
}
