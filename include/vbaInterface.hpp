#pragma once

#include "Option.hpp"
#include "EuropeanOption.hpp"
#include "AmericanOption.hpp"
#include "CallPayoff.hpp"
#include "PutPayoff.hpp"
#include "CrankNicolsonSolver.hpp"
#include "Greeks.hpp"
#include <memory>
#include <cstring>
#include <windows.h>
#include <oleauto.h>
#include <variant>

extern "C" {
    __declspec(dllexport) void computeCrankNicolsonGrid(
        int N, int M, double S, double T, double K,
        double sigma, double r_0, double r_1,
        const char* optionType, const char* exerciceType,
        SAFEARRAY**, SAFEARRAY**, SAFEARRAY**, SAFEARRAY**, SAFEARRAY**, SAFEARRAY**);

    /*__declspec(dllexport) void computeCrankNicolsonGreeks(
        int N, int M, double S, double T, double K,
        double sigma, double r_0, double r_1,
        const char* optionType, const char* exerciceType,
        SAFEARRAY** outGreeks);*/

    __declspec(dllexport) void CreateSafeArrayExample(int, SAFEARRAY**);
}