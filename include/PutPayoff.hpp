#pragma once
#include "PayOff.hpp"

class PutPayoff : public Payoff {
public:
	PutPayoff() = default;
	PutPayoff(double strike);
	~PutPayoff() override = default;
	double operator()(double spot) const override;
	std::string getType() const override;
	//Payoff* clone() const override;

};
