#pragma once
#include "Payoff.hpp"

class CallPayoff : public Payoff {
public:
	CallPayoff() = default;
	CallPayoff(double);
	~CallPayoff() override = default;
	//Payoff* clone() const override;
	double operator()(double) const override;
	std::string getType() const override;

};
