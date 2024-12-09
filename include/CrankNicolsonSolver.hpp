#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include "Option.hpp"

class CrankNicolsonSolver {
private:
	static void earlyExercise(const Option&, std::vector<std::vector<double>>&, int);
	static void setupBoundaryConditions(const Option&, std::vector<std::vector<double>>&);
public:
	static std::vector<std::vector<double>> solve(const Option&, int, int);
};
