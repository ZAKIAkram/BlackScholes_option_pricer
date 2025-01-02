
#pragma once
#include "Option.hpp"

class EuropeanOption : public Option {
public:
	EuropeanOption(const std::shared_ptr<Payoff>&, double, double, double, double);
	EuropeanOption(const EuropeanOption&);
	std::string getExerciseType() const override;
	std::shared_ptr<Option> clone() const override;
};
