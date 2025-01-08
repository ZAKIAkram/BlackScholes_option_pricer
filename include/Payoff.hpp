#pragma once
#include <iostream>

class Payoff {
protected:
	double _strike;
	Payoff() = default;
	Payoff(double strike);
public:
	double getStrike() const;
	virtual double operator()(double spot) const = 0;
	virtual ~Payoff() = default;
	virtual std::string getType() const = 0;
};
