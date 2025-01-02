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
	Greeks(int = 100, int = 100);
	Greeks(const Option&, const std::vector<std::vector<double>>&, int = 100, int = 100);
	std::vector<double> getDelta() const;
	std::vector<double> getGamma() const;
	std::vector<double> getRho() const;
	std::vector<double> getVega() const;
	std::vector<std::vector<double>> getTheta() const;
};