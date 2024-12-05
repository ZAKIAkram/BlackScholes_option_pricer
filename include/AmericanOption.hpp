#pragma once
#include "Option.hpp"

class AmericanOption : public Option{
public:
	std::string getExerciseType() const override {
		return "american";
	}
};
