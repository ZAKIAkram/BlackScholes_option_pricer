#include "Rates.hpp"

Rates::Rates(double rate) : _rate_0(rate), _rate_1(rate) {}

Rates::Rates(double rate_0, double rate_1) : _rate_0(rate_0), _rate_1(rate_1) {}

double Rates::at(double t) const {
	if (t < 0 or t > 1) {
		throw std::out_of_range("t is out of range");
	}
	return _rate_0 + (_rate_1 - _rate_0) * t;
}
