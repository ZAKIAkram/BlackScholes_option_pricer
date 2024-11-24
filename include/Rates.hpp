#pragma once

#include <iostream>
#include <unordered_map>

class Rates {
private:
	// time: rate, time is between 0 and 1
	std::unordered_map<double, double> _rates;
public:
	Rates() = default;
	Rates(double, double, int); // rate at 0, rate at 1, N
	Rates(double, double, double); // rate at 0, rate at 1, dt
	Rates(const std::unordered_map<double, double>&);
	~Rates() = default;
	double at(double) const;
};
