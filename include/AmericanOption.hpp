#pragma once
#include "VanillaOption.hpp"

class AmericanOption : public VanillaOption{
public:
	std::string getType() const override {
		return "american";
	}
};
