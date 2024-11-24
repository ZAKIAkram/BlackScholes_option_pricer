#pragma once

#include <iostream>
#include <unordered_map>

class Rates {
private:
	double _rate_0, _rate_1;
public:
	Rates() = default;
	Rates(double);
	Rates(double, double); // rate at 0, rate at 1
	~Rates() = default;
	double at(double) const;
};
