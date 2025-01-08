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
        int, int, double, double, double,
        double, double, double,
        const char*, const char*,
        SAFEARRAY**, SAFEARRAY**, SAFEARRAY**, SAFEARRAY**, SAFEARRAY**, SAFEARRAY**);
}