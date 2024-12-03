#pragma once
#include "OptionParameters.hpp"


// Constructor with parameters
OptionParameters::OptionParameters(ContractType contractType,
    ExerciseType exerciseType,
    double maturity,
    double strike,
    double computationDate,
    const std::vector<double>& timeMesh,
    const std::vector<double>& spotMesh,
    double spotPrice,
    const Rates& riskFreeRate,
    double volatility,
    double dividend)
    : contractType(contractType),
    exerciseType(exerciseType),
    maturity(maturity),
    strike(strike),
    computationDate(computationDate),
    timeMesh(timeMesh),
    spotMesh(spotMesh),
    spotPrice(spotPrice),
    riskFreeRate(riskFreeRate),
    volatility(volatility),
    dividend(dividend) {
}

// Getter functions
OptionParameters::ContractType OptionParameters::getContractType() const {
    return contractType;
}

OptionParameters::ExerciseType OptionParameters::getExerciseType() const {
    return exerciseType;
}

double OptionParameters::getMaturity() const {
    return maturity;
}

double OptionParameters::getStrike() const {
    return strike;
}

double OptionParameters::getComputationDate() const {
    return computationDate;
}

const std::vector<double>& OptionParameters::getTimeMesh() const {
    return timeMesh;
}

const std::vector<double>& OptionParameters::getSpotMesh() const {
    return spotMesh;
}

double OptionParameters::getSpotPrice() const {
    return spotPrice;
}

const Rates& OptionParameters::getRiskFreeRate() const {
    return riskFreeRate;
}

double OptionParameters::getVolatility() const {
    return volatility;
}

double OptionParameters::getDividend() const {
    return dividend;
}

