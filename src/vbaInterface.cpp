#include "vbaInterface.hpp"

void copyArray(const std::vector<double>& data, SAFEARRAY** outArray) {
    int M = data.size() - 1;
    double* pData;
    HRESULT hr = SafeArrayAccessData(*outArray, (void**)&pData);
    if (FAILED(hr)) {
        return;
    }
    for (int i = 0; i < M + 1; i++) {
        pData[i] = data[i];
    }
    SafeArrayUnaccessData(*outArray);
}

void flattenAndCopyArray(const std::vector<std::vector<double>>& data, SAFEARRAY** outArray) {

    int M = data.size() - 1;
    int N = data[0].size() - 1;

    double* pData;
    HRESULT hr = SafeArrayAccessData(*outArray, (void**)&pData);
    if (FAILED(hr)) {
        return;
    }
    for (int i = 0; i < M + 1; i++){
        for (int j = 0; j < N + 1; j++) {
            pData[i * (N + 1) + j] = data[i][j];
        }
    }
    SafeArrayUnaccessData(*outArray);
}

extern "C" {
    __declspec(dllexport) void computeCrankNicolsonGrid(
            int N, int M, double S, double T, double K, 
            double sigma, double r_0, double r_1,
            const char* optionType, const char* exerciceType,
            SAFEARRAY** outGrid, SAFEARRAY** outDelta, SAFEARRAY** outGamma, SAFEARRAY** outRho, SAFEARRAY** outVega, SAFEARRAY** outTheta) {
        try {
            std::shared_ptr<Payoff> payoff;
            if (strcmp(optionType, "call") == 0) {
                payoff = std::make_shared<CallPayoff>(K);
            }
            else if (strcmp(optionType, "put") == 0) {
                payoff = std::make_shared<PutPayoff>(K);
            }
            else {
                std::cout << "optionType should be either 'put' or 'call'" << std::endl;
                return;
            }

            std::unique_ptr<Option> option;
            if (strcmp(exerciceType, "European") == 0) {
                option = std::make_unique<EuropeanOption>(payoff, T, sigma, r_0, r_1);
            }
            else if (strcmp(exerciceType, "American") == 0) {
                option = std::make_unique<AmericanOption>(payoff, T, sigma, r_0, r_1);
            }
            else {
                std::cout << "exerciseType should be either 'American' or 'European'" << std::endl;
                return;
            }

            std::vector<std::vector<double>> resultGrid = CrankNicolsonSolver::solve(*option, N, M);
            
            flattenAndCopyArray(resultGrid, outGrid);

            Greeks greeks(*option, resultGrid, N, M);
            std::vector<double> delta = greeks.getDelta();
            std::vector<double> gamma = greeks.getGamma();
            std::vector<double> vega = greeks.getVega();
            std::vector<double> rho = greeks.getRho();
            std::vector<std::vector<double>> theta = greeks.getTheta();

            copyArray(delta, outDelta);
            copyArray(gamma, outGamma);
            copyArray(rho, outRho);
            copyArray(vega, outVega);

            flattenAndCopyArray(theta, outTheta);



        }
        catch (const std::exception& e) {
            std::cerr << "An error occurred: " << e.what() << std::endl;
            return;
        }
    }
}