#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include "Option.hpp"

class CrankNicolsonSolver {
private:
	std::vector<double> _time_grid; // to remove maybe
	std::vector<double> _spot_grid; // to remove maybe
	std::vector<std::vector<double>> _grid; // to remove, we should code this st the solver does not depend on the option
	size_t _N, _M; // time, spot

	std::vector<std::vector<double>>& setupDirichletBoundaryConditions(const std::shared_ptr<Option>&); // add const later TODO
	std::vector<std::vector<double>>& setupNeumanntBoundaryConditions(const std::shared_ptr<Option>&, std::vector<std::vector<double>>&) const; // TODO: return Void?
	void earlyExercise(const Option&, std::vector<std::vector<double>>&) const;

public:
	CrankNicolsonSolver() = default;
	CrankNicolsonSolver(size_t = 100, size_t = 100);

	std::vector<double> getTimeGrid() const;
	std::vector<double> getSpotGrid() const;
	int getN() const;
	int getM() const;

	std::vector<std::vector<double>> solve(const std::shared_ptr<Option>& option);

	// Solve the tridiagonal system for a given time step using the Crank-Nicholson scheme

	std::vector<std::vector<double>>& solveTridiagonalSystem(std::vector<std::vector<double>>&, int, const std::shared_ptr<Option>&);

};
