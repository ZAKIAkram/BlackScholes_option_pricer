#include "BS_Pricer.hpp"

// Helper function: CDF of standard normal distribution
double normalCDF(double x) {
    return 0.5 * std::erfc(-x * (1 / std::sqrt(2)));
}

// Black-Scholes formula for European options
double blackScholesFormula(double S, double K, double T, double t, double r, double sigma, double y, OptionParameters::ContractType type) {
    if (T < t) {
        throw std::invalid_argument("Maturity T must be greater than the current time t.");
    }

    double tau = T - t;   // Time to maturity
    double d1 = (std::log(S / K) + (r - y + 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
    double d2 = d1 - sigma * std::sqrt(tau);

    if (type == OptionParameters::CALL) {
        return S * std::exp(-y * tau) * normalCDF(d1) - K * std::exp(-r * tau) * normalCDF(d2);
    }
    else { // PUT option
        return K * std::exp(-r * tau) * normalCDF(-d2) - S * std::exp(-y * tau) * normalCDF(-d1);
    }
}

// Compute Black-Scholes price for all times in the time mesh
double blackScholesPrice(const OptionParameters& params) {
    if (params.getExerciseType() != OptionParameters::EUROPEAN) {
        throw std::invalid_argument("Black-Scholes formula is only valid for European options.");
    }

    // Interpolate risk-free rates based on the computation date
    double T0 = params.getComputationDate();
    double T = params.getMaturity();
    std::vector<double> rates;
    for (double t : params.getTimeMesh()) {
        double t_normalized = (t - T0) / (T - T0); // Normalize time to [0, 1]
        rates.push_back(params.getRiskFreeRate().at(t_normalized));
    }

    // Compute prices for each time in the time mesh
    for (size_t i = 0; i < params.getTimeMesh().size(); ++i) {
        double t = params.getTimeMesh()[i];
        double rate = rates[i];

        // Use Black-Scholes formula
        double price = blackScholesFormula(
            params.getSpotPrice(),
            params.getStrike(),
            params.getMaturity(),
            t,
            rate,
            params.getVolatility(),
            params.getDividend(),
            params.getContractType()
        );

        // Output the time in months and corresponding option price
        std::cout << "Time: " << (t * 12) << " months, Price: " << price << std::endl;
    }

    // Return the price for the computation date (t = T0)
    double rateAtT0 = params.getRiskFreeRate().at(0.0); // Rate at t = T0

    return blackScholesFormula(
        params.getSpotPrice(),
        params.getStrike(),
        params.getMaturity(),
        T0,
        rateAtT0,
        params.getVolatility(),
        params.getDividend(),
        params.getContractType()
    );
}
