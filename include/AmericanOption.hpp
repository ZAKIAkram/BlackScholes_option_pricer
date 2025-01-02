#pragma once
#include "Option.hpp"

class AmericanOption : public Option{
public:
	AmericanOption(const std::shared_ptr<Payoff>&, double, double, double, double);
	AmericanOption(const AmericanOption&);
	std::string getExerciseType() const override;
	std::shared_ptr<Option> clone() const override;
};
