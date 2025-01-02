#include "BlackScholesExplicit.hpp"

double BlackScholesSolver::blackScholesPrice(double S, const Option& option, int t, int N) {
    double T = option.getMaturity();
    double K = option.getStrike();
    double sigma = option.getSigma();
    Rates rates = option.getRates();
    double dt = T / static_cast<double>(N);
    double r = option.getRates().at(t * dt / T); 
    std::string optionType = option.getType();
    double timeToMaturity = T * (1.0 - t * dt); 

    if (timeToMaturity <= 0.0) {
        return option.computePayoff(S);
    }

    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * timeToMaturity) / (sigma * std::sqrt(timeToMaturity));
    double d2 = d1 - sigma * std::sqrt(timeToMaturity);

    double N_d1 = 0.5 * (1.0 + std::erf(d1 / std::sqrt(2.0)));
    double N_d2 = 0.5 * (1.0 + std::erf(d2 / std::sqrt(2.0)));
    double N_minus_d1 = 1.0 - N_d1;
    double N_minus_d2 = 1.0 - N_d2;

    if (optionType == "Call") {
        return S * N_d1 - K * std::exp(-r * timeToMaturity) * N_d2;
    }
    else {
        return K * std::exp(-r * timeToMaturity) * N_minus_d2 - S * N_minus_d1;
    }

}




std::vector<std::vector<double>> BlackScholesSolver::solve(const Option& option, int N, int M) {
    double T = option.getMaturity();
    double S_max = 2.0 * option.getStrike();
    double ds = S_max / static_cast<double>(M);

    std::vector<std::vector<double>> grid(M + 1, std::vector<double>(N + 1, 0.0));

    for (int t = 0; t <= N; ++t) {
        for (int i = 0; i <= M; ++i) {
            double S = i * ds;
            grid[i][t] = blackScholesPrice(S, option, t, N);
        }
    }
    return grid;
}

