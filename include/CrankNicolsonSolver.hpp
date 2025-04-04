/**
 * @file CrankNicolsonSolver.hpp
 * @brief Header file for the CrankNicolsonSolver class, which solves the option pricing problem using the Crank-Nicolson method.
 */

#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include "Option.hpp"

 /**
  * @class CrankNicolsonSolver
  * @brief Class for solving the option pricing problem using the Crank-Nicolson method.
  */
class CrankNicolsonSolver {
private:
    /**
     * @brief Set up the boundary conditions for the Crank-Nicolson grid.
     * @param option Option object for which to set up the boundary conditions.
     * @param grid grid to set up the boundary conditions on.
     */
	static void setupBoundaryConditions(const Option& option, std::vector<std::vector<double>>& grid);
public:
    CrankNicolsonSolver() = delete;
    /**
     * @brief Solve the option pricing problem using the Crank-Nicolson method.
     * @param option Option object to price.
     * @param N Number of time steps.
     * @param M Number of asset price steps.
     * @return 2D vector representing the option price grid.
     */
	static std::vector<std::vector<double>> solve(const Option& option, size_t N, size_t M);
};
