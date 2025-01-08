
#pragma once
#include "Option.hpp"

class EuropeanOption : public Option {
public:
	EuropeanOption(const std::shared_ptr<Payoff>& option, double maturity, double sigma, double rate_0, double rate_1);
	EuropeanOption(const EuropeanOption& other);
	std::string getExerciseType() const override;
	std::shared_ptr<Option> clone() const override;
};
