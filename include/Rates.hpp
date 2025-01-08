#pragma once

#include <iostream>
//#include <unordered_map>

class Rates {
private:
	double _rate_0, _rate_1;
public:
	Rates() = default;
	Rates(double rate_0, double rate_1); 
	Rates(const Rates& other);
	~Rates() = default;
	double at(double t) const;
};
