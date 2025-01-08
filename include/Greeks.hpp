/**
 * @file Greeks.hpp
 * @brief Header file for the Greeks class, which calculates and stores the Greeks of an option.
 */

#pragma once

#include <vector>
#include "Option.hpp"
#include "CrankNicolsonSolver.hpp"


/**
 * @class Greeks
 * @brief Class for calculating and storing the Greeks of an option.
 */

class Greeks {
private:
	int _N, _M;
	std::vector<double> _delta, _gamma, _rho, _vega;
	std::vector<std::vector<double>> _theta;
public:

	/**
	 * @brief constructor.
	 * @param N Number of time steps (default is 100).
	 * @param M Number of asset price steps (default is 100).
	 */
	Greeks(int N = 100, int M = 100);

	/**
	 * @brief constructor.
	 * @param option Option object.
	 * @param grid Grid of option prices.
	 * @param N Number of time steps (default is 100).
	 * @param M Number of asset price steps (default is 100).
	 */
	Greeks(const Option& option, const std::vector<std::vector<double>>& grid, int N = 100, int M = 100);
	std::vector<double> getDelta() const;
	std::vector<double> getGamma() const;
	std::vector<double> getRho() const;
	std::vector<double> getVega() const;
	std::vector<std::vector<double>> getTheta() const;
};