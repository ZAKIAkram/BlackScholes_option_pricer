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
	EuropeanOption(const std::shared_ptr<Payoff>&, double, double, double, double, int);
	std::string getExerciseType() const override;
};
