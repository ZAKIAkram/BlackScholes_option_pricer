#pragma once

#include <iostream>
#include <unordered_map>

class Rate {
private:
	// time: rate, time is between 0 and 1
	std::unordered_map<double, double> _rates;
public:
	Rate() = default;
	Rate(double, double, int); // rate at 0, rate at 1, N
	Rate(double, double, double); // rate at 0, rate at 1, dt
	Rate(const std::unordered_map<double, double>&);
	~Rate() = default;
	double at(double) const;
};
