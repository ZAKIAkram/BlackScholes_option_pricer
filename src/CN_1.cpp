#include "CN_1.hpp"
#include "BoundaryConditions.hpp"
#include <cmath>
#include <algorithm>

// Constructor
CrankNicolsonSolver::CrankNicolsonSolver(const OptionParameters& optionParams)
    : _optionParams(optionParams) {
    initializeGrid();
    computePDECoefficients();
}

// Initialize the grid with spot prices and time steps
void CrankNicolsonSolver::initializeGrid() {
    double strike = _optionParams.getStrike();
    double maturity = _optionParams.getMaturity();
    double spotPrice = _optionParams.getSpotPrice();

    // Define the number of time steps and spot grid points
    size_t numSpotSteps = 100;  
    size_t numTimeSteps = 100; 

    _spotGrid.resize(numSpotSteps);
    _timeGrid.resize(numTimeSteps);
    _grid.resize(numTimeSteps, std::vector<double>(numSpotSteps));

    double maxSpotPrice = strike * 3;  // max = 3*strike price 
    double dS = maxSpotPrice / (numSpotSteps - 1);  // Step size in spot price
    double dt = maturity / (numTimeSteps - 1);     // Step size in time

    // Initialize spot grid
    for (size_t i = 0; i < numSpotSteps; ++i) {
        _spotGrid[i] = i * dS;
    }

    // Initialize time grid
    for (size_t i = 0; i < numTimeSteps; ++i) {
        _timeGrid[i] = i * dt;
    }
}

// Compute PDE coefficients for Crank-Nicholson method
void CrankNicolsonSolver::computePDECoefficients() {
    double sigma = _optionParams.getVolatility();
    double r = _optionParams.getRiskFreeRate().at(0);
    double dS = _spotGrid[1] - _spotGrid[0];
    double dt = _timeGrid[1] - _timeGrid[0];

    _alpha.resize(_spotGrid.size());
    _beta.resize(_spotGrid.size());
    _gamma.resize(_spotGrid.size());

    for (size_t i = 1; i < _spotGrid.size() - 1; ++i) {
        double S = _spotGrid[i];
        double d1 = 0.5 * sigma * sigma * S * S;
        double d2 = r * S;

        _alpha[i] = 0.25 * dt * (sigma * sigma * S * S / (dS * dS) - d2 / dS);
        _beta[i] = -dt * 0.5 * (sigma * sigma * S * S / (dS * dS) + r);
        _gamma[i] = 0.25 * dt * (sigma * sigma * S * S / (dS * dS) + d2 / dS);
    }
}

// Solve the PDE using Crank-Nicholson method
std::vector<std::vector<double>> CrankNicolsonSolver::solve() {
    BoundaryConditions boundaryConditions(_optionParams);

    // Apply boundary conditions at the final time step
    boundaryConditions.apply(_grid, _spotGrid, _timeGrid);

    // Perform time-stepping to solve for option price at each grid point
    for (size_t tIndex = _timeGrid.size() - 2; tIndex > 0; --tIndex) {
        solveTridiagonal(tIndex);

        // Apply boundary conditions at each step
        boundaryConditions.apply(_grid, _spotGrid, _timeGrid);
    }

    return _grid;
}

// Solve the tridiagonal system for a given time step using the Crank-Nicholson scheme
void CrankNicolsonSolver::solveTridiagonal(size_t tIndex) {
    size_t N = _spotGrid.size();
    std::vector<double> L(N - 2), M(N - 2), R(N - 2);  // Tridiagonal matrix components

    for (size_t i = 1; i < N - 1; ++i) {
        // Crank-Nicholson scheme setup
        if (i == 1) {
            L[i - 1] = -_alpha[i];
            M[i - 1] = 1.0 + _beta[i];
            R[i - 1] = _gamma[i];
        }
        else if (i == N - 2) {
            L[i - 1] = -_gamma[i];
            M[i - 1] = 1.0 + _beta[i];
            R[i - 1] = _alpha[i];
        }
        else {
            L[i - 1] = -_alpha[i];
            M[i - 1] = 1.0 + _beta[i];
            R[i - 1] = -_gamma[i];
        }
    }

    // Forward substitution (solving L*U = b)
    for (size_t i = 1; i < N - 2; ++i) {
        double factor = L[i] / M[i - 1];
        M[i] -= factor * R[i - 1];
        R[i] -= factor * R[i - 1];
    }

    // Backward substitution (solving U = M^-1 * b)
    for (size_t i = N - 3; i > 0; --i) {
        _grid[tIndex][i] = (R[i] - L[i] * _grid[tIndex + 1][i + 1]) / M[i];
    }
}

// Accessor functions
std::vector<double> CrankNicolsonSolver::getSpotGrid() const {
    return _spotGrid;
}

std::vector<double> CrankNicolsonSolver::getTimeGrid() const {
    return _timeGrid;
}

std::vector<std::vector<double>> CrankNicolsonSolver::getGrid() const {
    return _grid;
}
