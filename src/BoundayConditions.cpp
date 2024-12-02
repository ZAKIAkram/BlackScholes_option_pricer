#include "BoundaryConditions.hpp"
#include <algorithm>

// Constructor
BoundaryConditions::BoundaryConditions(const OptionParameters& optionParams)
    : _optionParams(optionParams) {
}

// Apply boundary conditions
void BoundaryConditions::apply(std::vector<std::vector<double>>& grid,
    const std::vector<double>& spotGrid,
    const std::vector<double>& timeGrid) {
    applyLowerBoundary(grid, timeGrid);
    applyUpperBoundary(grid, spotGrid);

    if (_optionParams.getExerciseType() == OptionParameters::AMERICAN) {
        applyEarlyExercise(grid, spotGrid);
    }
}

// Lower boundary condition
void BoundaryConditions::applyLowerBoundary(std::vector<std::vector<double>>& grid,
    const std::vector<double>& timeGrid) {
    for (size_t i = 0; i < timeGrid.size(); ++i) {
        grid[i][0] = 0.0; // Value is zero at S = 0 for both call and put
    }
}

// Upper boundary condition
void BoundaryConditions::applyUpperBoundary(std::vector<std::vector<double>>& grid,
    const std::vector<double>& spotGrid) {
    size_t M = spotGrid.size() - 1; // Last spot index
    for (size_t i = 0; i < grid.size(); ++i) {
        if (_optionParams.getContractType() == OptionParameters::CALL) {
            grid[i][M] = spotGrid[M] - _optionParams.getStrike(); // Intrinsic value for call
        }
        else {
            grid[i][M] = 0.0; // Value for put as S -> infinity
        }
    }
}

// Early exercise condition
void BoundaryConditions::applyEarlyExercise(std::vector<std::vector<double>>& grid,
    const std::vector<double>& spotGrid) {
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < spotGrid.size(); ++j) {
            if (_optionParams.getContractType() == OptionParameters::CALL) {
                grid[i][j] = std::max(grid[i][j], spotGrid[j] - _optionParams.getStrike());
            }
            else {
                grid[i][j] = std::max(grid[i][j], _optionParams.getStrike() - spotGrid[j]);
            }
        }
    }
}
