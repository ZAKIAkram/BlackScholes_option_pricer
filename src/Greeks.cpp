#include "CN_1.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm> // For std::lower_bound
#include "Greeks.hpp"

Greeks computeGreeks(CrankNicolsonSolver& solver, double spotPrice, double deltaS, double deltaT, double deltaSigma, double deltaR) {
    // Retrieve the grids
    const auto& spotGrid = solver.getSpotGrid();
    const auto& timeGrid = solver.getTimeGrid();
    const auto& priceGrid = solver.getGrid();

    // Ensure non-zero increments
    if (deltaS == 0 || deltaT == 0 || deltaSigma == 0 || deltaR == 0) {
        throw std::runtime_error("Increments deltaS, deltaT, deltaSigma, and deltaR must be non-zero.");
    }

    // Locate the current spot price index using std::lower_bound
    auto it = std::lower_bound(spotGrid.begin(), spotGrid.end(), spotPrice);
    if (it == spotGrid.end() || it == spotGrid.begin()) {
        throw std::runtime_error("Spot price out of bounds in the grid.");
    }
    size_t spotIndex = std::distance(spotGrid.begin(), it);

    // Use the first time step for Delta, Gamma, Theta (t = 0)
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

    // Compute Vega
    double originalVolatility = solver.getOptionParameters().getVolatility();

    // Modify volatility
    auto modifiedParams = solver.getOptionParameters();
    modifiedParams.setVolatility(originalVolatility + deltaSigma);
    CrankNicolsonSolver solverPlusSigma(modifiedParams);
    double V_plusSigma = solverPlusSigma.solve()[timeIndex][spotIndex];

    modifiedParams.setVolatility(originalVolatility - deltaSigma);
    CrankNicolsonSolver solverMinusSigma(modifiedParams);
    double V_minusSigma = solverMinusSigma.solve()[timeIndex][spotIndex];

    double vega = (V_plusSigma - V_minusSigma) / (2 * deltaSigma);

    // Compute Rho
    double originalRate = solver.getOptionParameters().getRiskFreeRate().at(0);

    // Modify risk-free rate
    modifiedParams.setRiskFreeRate({ originalRate + deltaR });
    CrankNicolsonSolver solverPlusR(modifiedParams);
    double V_plusR = solverPlusR.solve()[timeIndex][spotIndex];

    modifiedParams.setRiskFreeRate({ originalRate - deltaR });
    CrankNicolsonSolver solverMinusR(modifiedParams);
    double V_minusR = solverMinusR.solve()[timeIndex][spotIndex];

    double rho = (V_plusR - V_minusR) / (2 * deltaR);

    // Return Greeks
    return { delta, gamma, theta, vega, rho };
}
