#pragma once

#include <vector>
#include "OptionParameters.hpp"
#include "Rates.hpp"

// Crank-Nicholson solver for option pricing
class CrankNicolsonSolver {
public:
    const OptionParameters& getOptionParameters() const {
        return _optionParams;
    }
    // Constructor to initialize with the option parameters
    CrankNicolsonSolver(const OptionParameters& optionParams);

    // Solve the PDE and return the option price grid
    std::vector<std::vector<double>> solve();

    // Accessor functions
    std::vector<double> getSpotGrid() const;
    std::vector<double> getTimeGrid() const;
    std::vector<std::vector<double>> getGrid() const;

private:
    const OptionParameters& _optionParams;  // Reference to option parameters
    std::vector<double> _spotGrid;          // Spot price grid
    std::vector<double> _timeGrid;          // Time grid
    std::vector<std::vector<double>> _grid; // Solution grid
    std::vector<double> _alpha, _beta, _gamma; // PDE coefficients

    // Initialize grid for spot prices and time steps
    void initializeGrid();

    // Compute PDE coefficients for Crank-Nicholson
    void computePDECoefficients();

    // Solve the tridiagonal system at each time step
    void solveTridiagonal(size_t tIndex);

};
