#pragma once

#include <iostream>
#include <vector>
#include "VanillaOption.hpp"

class CrankNicolsonSolver {
private:
	std::vector<double> _time_grid;
	std::vector<double> _spot_grid;
	std::vector<std::vector<double>> _grid;
	int _N, _M; // time, spot
	//std::shared_ptr<VanillaOption> _option;

	//double _S_max; // wait for strike price to use 3 * K or define it here ?
	void setupDirichletBoundaryConditions(const std::shared_ptr<VanillaOption>&); // dirichlet
public:
	CrankNicolsonSolver() = default;
	CrankNicolsonSolver(int, int);

	std::vector<double> getTimeGrid() const;
	std::vector<double> getSpotGrid() const;
	int getN() const;
	int getM() const;

	std::vector<double> solve(VanillaOption& option) const;

};
