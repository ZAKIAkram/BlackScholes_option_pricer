#include "vbaInterface.hpp"


//extern "C" {
//    __declspec(dllexport)
//        void computeCrankNicolson(int N, int M, double S, double T, double K, double sigma, double r_0, double r_1,
//            const char* optionType, const char* exerciceType, SAFEARRAY* grid, SAFEARRAY* delta, SAFEARRAY* gamma, SAFEARRAY* rho, SAFEARRAY* vega, SAFEARRAY* theta) {
//        try {
//            std::shared_ptr<Payoff> payoff;
//            if (strcmp(optionType, "call") == 0) {
//                payoff = std::make_shared<CallPayoff>(K);
//            }
//            else if (strcmp(optionType, "put") == 0) {
//                payoff = std::make_shared<PutPayoff>(K);
//            }
//            else {
//                throw std::invalid_argument("Invalid option type. Use 'call' or 'put'.");
//            }
//
//            std::unique_ptr<Option> option;
//            if (strcmp(exerciceType, "European") == 0) {
//                option = std::make_unique<EuropeanOption>(payoff, T, sigma, r_0, r_1);
//            }
//            else if (strcmp(exerciceType, "American") == 0) {
//                option = std::make_unique<AmericanOption>(payoff, T, sigma, r_0, r_1);
//            }
//            else {
//                throw std::invalid_argument("Invalid exercise type. Use 'European' or 'American'.");
//            }
//
//            std::vector<std::vector<double>> resultGrid = CrankNicolsonSolver::solve(*option, N, M);
//
//            double* gridData = (double*)grid->pvData;
//
//            for (int i = 0; i < M; i++) {
//                for (int j = 0; j < N; j++) {
//                    gridData[i * N + j] = resultGrid[i][j];
//                    //std::cout << grid[i * N + j] << " : " << resultGrid[i][j] << std::endl;
//                }
//            }
//
//            Greeks greeks(*option, resultGrid, N, M);
//            std::vector<double> cnDelta = greeks.getDelta();
//            std::vector<double> cnGamma = greeks.getGamma();
//            std::vector<double> cnVega = greeks.getVega();
//            std::vector<double> cnRho = greeks.getRho();
//            std::vector<std::vector<double>> cnTheta = greeks.getTheta();
//
//
//
//
//
//            for (int i = 0; i < M; ++i) {
//                delta[i] = cnDelta[i];
//                gamma[i] = cnGamma[i];
//                rho[i] = cnRho[i];
//                vega[i] = cnVega[i];
//            }
//
//
//            for (int i = 0; i < M; i++) {
//                for (int j = 0; j < N; j++) {
//                    theta[i * N + j] = cnTheta[i][j];
//                }
//            }
//        }
//        catch (const std::exception& ex) {
//            std::cerr << "Error in computeCrankNicolson function: " << ex.what() << std::endl;
//        }
//    }
//}






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
                //outGrid = nullptr;
                // TODO : throw exception
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
                //outGrid = nullptr;
                // TODO: Throw exception
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
        catch (...) {
            //outGrid = nullptr;
            // TODO
            return;
        }
    }

    //__declspec(dllexport) void computeCrankNicolsonGreeks(
    //        int N, int M, double S, double T, double K, 
    //        double sigma, double r_0, double r_1,
    //        const char* optionType, const char* exerciceType,
    //        SAFEARRAY* outGreeks) {
    //    try {
    //        std::shared_ptr<Payoff> payoff;
    //        if (strcmp(optionType, "call") == 0) {
    //            payoff = std::make_shared<CallPayoff>(K);
    //        }
    //        else if (strcmp(optionType, "put") == 0) {
    //            payoff = std::make_shared<PutPayoff>(K);
    //        }
    //        else {
    //            outGreeks = nullptr;
    //            return;
    //        }

    //        std::unique_ptr<Option> option;
    //        if (strcmp(exerciceType, "European") == 0) {
    //            option = std::make_unique<EuropeanOption>(payoff, T, sigma, r_0, r_1);
    //        }
    //        else if (strcmp(exerciceType, "American") == 0) {
    //            option = std::make_unique<AmericanOption>(payoff, T, sigma, r_0, r_1);
    //        }
    //        else {
    //            outGreeks = nullptr;
    //            return;
    //        }

    //        // Solve and compute Greeks
    //        std::vector<std::vector<double>> resultGrid = CrankNicolsonSolver::solve(*option, N, M);
    //        Greeks greeks(*option, resultGrid, N, M);

    //        // Combine all Greeks into one flat array
    //        std::vector<double> allGreeks;
    //        allGreeks.reserve(4*M + M*N);

    //        auto delta = greeks.getDelta();
    //        auto gamma = greeks.getGamma();
    //        auto rho = greeks.getRho();
    //        auto vega = greeks.getVega();
    //        auto theta = greeks.getTheta();

    //        allGreeks.insert(allGreeks.end(), delta.begin(), delta.end());
    //        allGreeks.insert(allGreeks.end(), gamma.begin(), gamma.end());
    //        allGreeks.insert(allGreeks.end(), rho.begin(), rho.end());
    //        allGreeks.insert(allGreeks.end(), vega.begin(), vega.end());
    //        
    //        for (int i = 0; i < M; i++) {
    //            allGreeks.insert(allGreeks.end(), theta[i].begin(), theta[i].end());
    //        }

    //        flattenAndCopyArray(allGreeks, outGreeks);
    //    }
    //    catch (...) {
    //        outGreeks = nullptr;
    //    }
    //}

    __declspec(dllexport) void CreateSafeArrayExample(int N, SAFEARRAY** outArray) {
        double* pData;
        HRESULT hr = SafeArrayAccessData(*outArray, (void**)&pData);
        if (FAILED(hr)) {
            return;
        }

        for (int i = 0; i < N; ++i) {
            pData[i] = static_cast<double>(i + 1);
        }

        SafeArrayUnaccessData(*outArray);
    }
}