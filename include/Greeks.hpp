#pragma once
#include <vector>


struct Greeks {
	std::vector<double> delta, gamma, theta, rho, vega;
	Greeks(const std::vector<std::vector<double>>&);
};