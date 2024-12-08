#include "Americanoption.hpp"

AmericanOption::AmericanOption(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma, double rate_0, double rate_1, int N) : Option(payoff, maturity, sigma, rate_0, rate_1, N) {}

std::string AmericanOption::getExerciseType() const {
	return "American";
}
