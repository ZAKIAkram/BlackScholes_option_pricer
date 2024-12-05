#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include "Option.hpp"

class CrankNicolsonSolver {
private:
	std::vector<double> _time_grid; // to remove maybe
	std::vector<double> _spot_grid; // to remove maybe
	std::vector<std::vector<double>> _grid; // to remove, we should code this st the solver does not depend on the option
	size_t _N, _M; // time, spot

	std::vector<std::vector<double>>& setupDirichletBoundaryConditions(const Option&) const;
	std::vector<std::vector<double>>& setupNeumanntBoundaryConditions(const Option&, std::vector<std::vector<double>>&) const; // TODO: return Void?
	void earlyExercise(const Option&, std::vector<std::vector<double>>&) const;
public:
	CrankNicolsonSolver() = default;
	CrankNicolsonSolver(size_t = 100, size_t = 100);

	std::vector<double> getTimeGrid() const;
	std::vector<double> getSpotGrid() const;
	int getN() const;
	int getM() const;

	std::vector<std::vector<double>> solve(const Option& option) const;

};
