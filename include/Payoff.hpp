#pragma once
#include <iostream>

class Payoff {
protected:
	double _strike;
	Payoff() = default;
	Payoff(double);
public:
	double getStrike() const;
	virtual double operator()(double) const = 0;
	virtual ~Payoff() = default;
	virtual std::string getType() const = 0;
};
