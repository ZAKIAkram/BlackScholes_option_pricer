#pragma once
#include "Option.hpp"

class AmericanOption : public Option{
public:
	AmericanOption(const std::shared_ptr<Payoff>&, double, double, double, double);
	std::string getExerciseType() const override;
};
