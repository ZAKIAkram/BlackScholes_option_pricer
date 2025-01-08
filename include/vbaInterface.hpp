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

extern "C" {
    __declspec(dllexport) void computeCrankNicolsonGrid(
        int N, int M, double T, double K, double sigma,
        double, double, double,
        const char*, const char*,
        SAFEARRAY**, SAFEARRAY**, SAFEARRAY**, SAFEARRAY**, SAFEARRAY**, SAFEARRAY**);
}

void copyArray(const std::vector<double>& data, SAFEARRAY** outArray);
void flattenAndCopyArray(const std::vector<std::vector<double>>& data, SAFEARRAY** outArray);