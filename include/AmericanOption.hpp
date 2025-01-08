#pragma once

#include "Option.hpp"

class AmericanOption : public Option{
public:
	AmericanOption(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma, double rate_0, double rate_1);
	AmericanOption(const AmericanOption& other);
	std::string getExerciseType() const override;
	std::shared_ptr<Option> clone() const override;
};
