#include "CrankNicolsonSolver.hpp"

std::vector<std::vector<double>> CrankNicolsonSolver::solve(const Option& option, size_t N, size_t M) {

    double T = option.getMaturity();
    double K = option.getStrike();
    Rates rates = option.getRates();
    double sigma = option.getSigma();
    double S_max = 2. * K;
    double dt = T / static_cast<double>(N);
    double ds = S_max / static_cast<double>(M);
    std::string optionType = option.getType();
    std::string exerciseType = option.getExerciseType();

    
    std::vector<double> a(M + 1, 0.0);  
    std::vector<double> b(M + 1, 0.0);
    std::vector<double> c(M + 1, 0.0);
    std::vector<double> d(M + 1, 0.0);

    std::vector<std::vector<double>> grid(M + 1, std::vector<double>(N + 1, 0.));

    // boundary conditions
    for (size_t i = 0; i <= M; ++i) {
        double spot = static_cast<double>(i) * ds;
        grid[i][N] = option.computePayoff(spot);
    }

    for (int t = N - 1; t >= 0; --t) {
        double r = rates.at(t * dt / T);

        
        for (size_t i = 0; i <= M; ++i) {
            double stockPrice = static_cast<double>(i) * ds;
            double alpha = 0.25 * dt * i * (sigma * sigma * i - r);
            double beta = -0.5 * dt * (sigma * sigma * i * i + r);
            double gamma = 0.25 * dt * i * (sigma * sigma * i + r);

            
            if (i > 0 and i < M) {
                a[i] = -alpha;
                b[i] = 1. - beta;
                c[i] = -gamma;
                d[i] = alpha * grid[i - 1][t + 1] + (1. + beta) * grid[i][t + 1] + gamma * grid[i + 1][t + 1];
            }
        }

        
        b[0] = 1.0;
        c[0] = -1.0;
        d[0] = 0.0;  

        
        a[M] = -1.0;
        b[M] = 1.0;
        if (optionType == "call") {
            d[M] = ds;  
        }
        else {
            d[M] = 0.0;  
        }

        for (size_t i = 1; i <= M; ++i) {
            double m = a[i] / b[i - 1];
            b[i] -= m * c[i - 1];
            d[i] -= m * d[i - 1];
        }

        grid[M][t] = d[M] / b[M];
        for (int i = M - 1; i >= 0; --i) {
            double value = (d[i] - c[i] * grid[i + 1][t]) / b[i];
            grid[i][t] = (std::abs(value) > 1e-16) ? value : 0.;
        }

        if (exerciseType == "American") {
            for (size_t i = 0; i <= M; ++i) {
                double spot = i * ds;
                grid[i][t] = std::max(grid[i][t], option.computePayoff(spot));
            }
        }
    }

    return grid;
}
