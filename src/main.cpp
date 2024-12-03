//#include "BS_pricer.hpp"
//#include "Rates.hpp"
//#include "Greeks.hpp"
//
//#include <iostream>
//
//int main() {
//    // Create the risk-free rate object (linear interpolation between 3% at t=0 and 5% at t=1)
//    Rates riskFreeRates(0.03, 0.05);
//
//    // Define option parameters
//    OptionParameters params = {
//        OptionParameters::PUT,          // ContractType (CALL or PUT)
//        OptionParameters::EUROPEAN,
//        2.0,                             // Maturity T (1 year)
//        100.0,                           // Strike price
//        0.0,                             // Computation date T0 (start at time 0)
//        {0.0, 0.25, 0.5, 0.75, 1.0},     // Time mesh (in years)
//        {50, 75, 100, 125, 150},         // Spot mesh (not used in pricing here)
//        100.0,                           // Current spot price S0
//        riskFreeRates,                   // Risk-free rate object
//        0.2,                             // Volatility (20%)
//        0                                //dividend
//    };
//
//    try {
//        //std::cout << "Option type: " << ExerciseType, ContractType<< std::endl;
//
//        
//        // Calculate Black-Scholes price at computation date (T0)
//        double priceAtT0 = blackScholesPrice(params);
//        //calculate the greeks
//        calculateGreeks(params);
//        // Output the result
//        //std::cout << "Option price at computation date (T0): " << priceAtT0 << std::endl;
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//    }
//
//    return 0;
//}
 
#pragma once
#include <vector>
#include <algorithm>
#include "OptionParameters.hpp"
#include "BoundaryConditions.hpp"
#include "CN_1.hpp"
#include "Greeks.hpp"



int main() {
    Rates riskFreeRates(0.03, 0.05);

        OptionParameters optionParams = {
        OptionParameters::PUT,          // ContractType (CALL or PUT)
        OptionParameters::AMERICAN,      // ExerciseType (EUROPEAN or AMERICAN)
        2.0,                             // Maturity T (1 year)
        100.0,                           // Strike price
        0.0,                             // Computation date T0 (start at time 0)
        {0.0, 0.25, 0.5, 0.75, 1.0},     // Time mesh (in years)
        {50, 75, 100, 125, 150},         // Spot mesh (not used in pricing here)
        100.0,                           // Current spot price S0
        riskFreeRates,                   // Risk-free rate object
        0.2,                             // Volatility (20%)
        0                                //dividend
    };
 

    // Create CrankNicolsonSolver object with option parameters
    CrankNicolsonSolver solver(optionParams);

    // Solve the PDE
    std::vector<std::vector<double>> optionPriceGrid = solver.solve();

    // Define the spot price for which we want prices across time steps
    double targetSpotPrice = 100.0;

    // Find the index of the target spot price in the spot grid
    const std::vector<double>& spotGrid = solver.getSpotGrid();
    size_t spotIndex = std::distance(spotGrid.begin(),
        std::find(spotGrid.begin(), spotGrid.end(), targetSpotPrice));

    if (spotIndex >= spotGrid.size()) {
        std::cerr << "Error: Target spot price not found in the spot grid!" << std::endl;
        return -1;
    }
    // Retrieve time grid for iteration
    const std::vector<double>& timeGrid = solver.getTimeGrid();

    // Define parameters for Greek computation
    double deltaS = 1e-2;  // Small change in spot price for finite difference
    double deltaT = timeGrid[1] - timeGrid[0];  // Time step size from the grid

    // Print headers
    std::cout << "Time Step\tOption Price\tDelta\tGamma\tTheta" << std::endl;

    // Compute option prices and Greeks at different time steps
    //mazal  nqad time steps 0, 0.25, 0.5, 0.75, 1
    for (size_t t = 0; t < timeGrid.size() - 1; ++t) {
        try {
            // Extract option price at the target spot price for the current time step
            double optionPrice = optionPriceGrid[t][spotIndex];

            // Compute Greeks for the current time step
            Greeks greeks = computeGreeks(solver, targetSpotPrice, deltaS, deltaT);

            // Display the results
            std::cout << timeGrid[t] << "\t\t"
                << optionPrice << "\t\t"
                << greeks.delta << "\t"
                << greeks.gamma << "\t"
                << greeks.theta << std::endl;

        }
        catch (const std::exception& e) {
            std::cerr << "Error computing Greeks at time step " << t
                << ": " << e.what() << std::endl;
        }
    }

    return 0;
}