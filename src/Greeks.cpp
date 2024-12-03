#include "CN_1.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>
#include "Greeks.hpp"


Greeks computeGreeks(CrankNicolsonSolver& solver, double spotPrice, double deltaS, double deltaT) {
    // Retrieve the grids
    const auto& spotGrid = solver.getSpotGrid();
    const auto& timeGrid = solver.getTimeGrid();
    const auto& priceGrid = solver.getGrid();

    // Locate the current spot price index
    size_t spotIndex = 0;
    while (spotIndex < spotGrid.size() && spotGrid[spotIndex] < spotPrice) {
        ++spotIndex;
    }
    if (spotIndex == 0 || spotIndex >= spotGrid.size() - 1) {
        throw std::runtime_error("Spot price out of bounds in the grid.");
    }

    // Use the first time step for Delta and Gamma (t = 0)
    size_t timeIndex = 0;

    // Extract values for Delta and Gamma
    double V_S = priceGrid[timeIndex][spotIndex];
    double V_S_plus = priceGrid[timeIndex][spotIndex + 1];
    double V_S_minus = priceGrid[timeIndex][spotIndex - 1];

    // Calculate Delta and Gamma
    double delta = (V_S_plus - V_S_minus) / (2 * deltaS);
    double gamma = (V_S_plus - 2 * V_S + V_S_minus) / (deltaS * deltaS);

    // Use the first two time steps for Theta (difference over time)
    if (timeGrid.size() < 2) {
        throw std::runtime_error("Not enough time steps to compute Theta.");
    }
    double V_T = priceGrid[timeIndex][spotIndex];
    double V_T_next = priceGrid[timeIndex + 1][spotIndex];
    double theta = (V_T_next - V_T) / deltaT;

    return { delta, gamma, theta };
}