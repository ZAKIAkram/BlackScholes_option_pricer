#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include "Option.hpp"

class CrankNicolsonSolver {
private:
	static void setupBoundaryConditions(const Option&, std::vector<std::vector<double>>&);
	static void earlyExerciseParallel(const Option& option,
		std::vector<std::vector<double>>& grid,
		int t,
		unsigned int num_threads);
public:
	static std::vector<std::vector<double>> solve(const Option&, int, int);
};
