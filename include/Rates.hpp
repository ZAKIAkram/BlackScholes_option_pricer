#pragma once

#include <iostream>
//#include <unordered_map>

class Rates {
private:
	double _rate_0, _rate_1, _eps;
public:
	Rates() = default;
	Rates(double, double, double = 0.); // rate at 0, rate at 1, epsilon
	Rates(const Rates&, double);
	~Rates() = default;
	double at(double) const;
};
