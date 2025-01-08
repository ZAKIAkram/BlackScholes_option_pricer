#pragma once

#include <vector>
#include "Option.hpp"
#include "CrankNicolsonSolver.hpp"


class Greeks {
private:
	int _N, _M;
	std::vector<double> _delta, _gamma, _rho, _vega;
	std::vector<std::vector<double>> _theta;
public:
	Greeks(int N = 100, int M = 100);
	Greeks(const Option& option, const std::vector<std::vector<double>>& grid, int N = 100, int M = 100);
	std::vector<double> getDelta() const;
	std::vector<double> getGamma() const;
	std::vector<double> getRho() const;
	std::vector<double> getVega() const;
	std::vector<std::vector<double>> getTheta() const;
};