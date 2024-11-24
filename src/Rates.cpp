#include "Rate.hpp"


Rates::Rates(double rate_0, double rate_1, int N) {
	if (N == 0) {
		throw std::invalid_argument("N should not be 0");
	}
	double dt = 1 / static_cast<double>(N);
	for (int i = 0; i < N; i++) {
		// linear interpolation
		_rates[i * dt] = rate_0 + (rate_1 - rate_0) * (i * dt);
	}
}

Rates::Rates(double rate_0, double rate_1, double dt) {
	if (dt <= 0 or dt >= 1) {
		throw std::invalid_argument("dt should be between 0 and 1");
	}
	int k = static_cast<int>(1 / dt);
	for (int i = 0; i <= k; i++) {
		// linear interpolation
		_rates[i * dt] = rate_0 + (rate_1 - rate_0) * (i * dt);
	}
}

Rates::Rates(const std::unordered_map<double, double>& rates) : _rates(rates) {}

double Rates::at(double t) const {
	if (t < 0 or t > 1) {
		throw std::out_of_range("t is out of range");
	}
	auto it = _rates.find(t);
	if (it == _rates.end()) {
		throw std::out_of_range("rate not found for the given time");
	}
	return it->second;
}

