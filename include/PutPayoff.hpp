#pragma once
#include "PayOff.hpp"

class PutPayoff : public Payoff {
public:
	PutPayoff() = default;
	PutPayoff(double);
	~PutPayoff() override = default;
	double operator()(double) const override;
	//Payoff* clone() const override;

};
