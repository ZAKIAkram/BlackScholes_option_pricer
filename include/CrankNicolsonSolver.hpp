#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include "Option.hpp"

class CrankNicolsonSolver {
private:
	static void setupBoundaryConditions(const Option& option, std::vector<std::vector<double>>& grid);
public:
	static std::vector<std::vector<double>> solve(const Option& option, int N, int M);
};
