#pragma once

#include <vector>
#include "OptionParameters.hpp"

//The boundary conditions are the payoffs of the option at the extreme points on the scheme


class BoundaryConditions {
public:
    BoundaryConditions(const OptionParameters& optionParams);

    // Apply boundary conditions to the grid
    void apply(std::vector<std::vector<double>>& grid,
        const std::vector<double>& spotGrid,
        const std::vector<double>& timeGrid);

private:
    const OptionParameters& _optionParams; // Reference to shared parameters

    void applyLowerBoundary(std::vector<std::vector<double>>& grid,
        const std::vector<double>& timeGrid);
    void applyUpperBoundary(std::vector<std::vector<double>>& grid,
        const std::vector<double>& spotGrid);
    void applyEarlyExercise(std::vector<std::vector<double>>& grid,
        const std::vector<double>& spotGrid);
};

