/*****************************************************************//**
 * \file   EuropeanOption.hpp
 * \brief  
 * 
 * \author akram
 * \date   November 2024
 *********************************************************************/
#pragma once
#include "Option.hpp"

class EuropeanOption : public Option {
public:
	std::string getExerciseType() const override {
		return "european";
	}
};
