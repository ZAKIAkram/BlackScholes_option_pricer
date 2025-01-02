#include "vbaInterface.hpp"


extern "C" {
    __declspec(dllexport)
        void computeCrankNicolson(int N, int M, double S, double T, double K, double sigma, double r_0, double r_1,
            const char* optionType, const char* exerciceType, double** grid, double* delta, double* gamma, double* rho, double* vega, double** theta) {
        try {
            std::shared_ptr<Payoff> payoff;
            if (strcmp(optionType, "call") == 0) {
                payoff = std::make_shared<CallPayoff>(K);
            }
            else if (strcmp(optionType, "put") == 0) {
                payoff = std::make_shared<PutPayoff>(K);
            }
            else {
                throw std::invalid_argument("Invalid option type. Use 'call' or 'put'.");
            }

            std::unique_ptr<Option> option;
            if (strcmp(exerciceType, "European") == 0) {
                option = std::make_unique<EuropeanOption>(payoff, T, sigma, r_0, r_1);
            }
            else if (strcmp(exerciceType, "American") == 0) {
                option = std::make_unique<AmericanOption>(payoff, T, sigma, r_0, r_1);
            }
            else {
                throw std::invalid_argument("Invalid exercise type. Use 'European' or 'American'.");
            }

            std::vector<std::vector<double>> resultGrid = CrankNicolsonSolver::solve(*option, N, M);

            

            for (int i = 0; i < M; ++i) {
                for (int j = 0; j < N; ++j) {
                    grid[i][j] = resultGrid[i][j];
                }
            }

            Greeks greeks(*option, resultGrid, N, M);
            std::vector<double> cnDelta = greeks.getDelta();
            std::vector<double> cnGamma = greeks.getGamma();
            std::vector<double> cnVega = greeks.getVega();
            std::vector<double> cnRho = greeks.getRho();
            std::vector<std::vector<double>> cnTheta = greeks.getTheta();





            for (int i = 0; i < M; ++i) {
                delta[i] = cnDelta[i];
                gamma[i] = cnGamma[i];
                rho[i] = cnRho[i];
                vega[i] = cnVega[i];
            }


            for (int i = 0; i < M; ++i) {
                for (int j = 0; j < N; ++j) {
                    theta[i][j] = cnTheta[i][j];
                }
            }
        }
        catch (const std::exception& ex) {
            std::cerr << "Error in computeCrankNicolson function: " << ex.what() << std::endl;
        }
    }
}
