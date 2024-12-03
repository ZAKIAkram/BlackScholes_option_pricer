#pragma once
#include <iostream>

class Payoff {
protected:
	double _strike;
	Payoff() = default;
	Payoff(double);
public:
	virtual double operator()(double) const = 0;
	virtual ~Payoff() = default;
	virtual std::string getType() const = 0;
	//virtual Payoff* clone() const = 0;
};
