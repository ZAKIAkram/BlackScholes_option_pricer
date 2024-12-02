#pragma once

#include <vector>
#include "Rates.hpp"

class OptionParameters {
public:
    // Enums for Contract Type (CALL/PUT) and Exercise Type (EUROPEAN/AMERICAN)
    enum ContractType { CALL, PUT };
    enum ExerciseType { EUROPEAN, AMERICAN };

    // Default constructor
    //OptionParameters();

    // Constructor with parameters
    OptionParameters(ContractType contractType,
        ExerciseType exerciseType,
        double maturity,
        double strike,
        double computationDate,
        const std::vector<double>& timeMesh,
        const std::vector<double>& spotMesh,
        double spotPrice,
        const Rates& riskFreeRate,
        double volatility,
        double dividend);

    // Getters for all private members
    ContractType getContractType() const;
    ExerciseType getExerciseType() const;
    double getMaturity() const;
    double getStrike() const;
    double getComputationDate() const;
    const std::vector<double>& getTimeMesh() const;
    const std::vector<double>& getSpotMesh() const;
    double getSpotPrice() const;
    const Rates& getRiskFreeRate() const;
    double getVolatility() const;
    double getDividend() const;

private:
    // Option contract and exercise type
    ContractType contractType;
    ExerciseType exerciseType;

    // Option details
    double maturity;
    double strike;
    double computationDate;
    std::vector<double> timeMesh; // Time mesh parameters
    std::vector<double> spotMesh; // Spot mesh parameters
    double spotPrice;
    Rates riskFreeRate;           // Risk-free rate object
    double volatility;
    double dividend;
};
