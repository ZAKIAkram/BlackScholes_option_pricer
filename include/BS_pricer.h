#pragma once

#include <iostream>
#include <string>

//Library
#include <cmath>

// CDF of normal distribution
double normalCDF(double x);

// Black-scholes
double blackScholesPrice(char optionType, double S, double K, double T, double r, double sigma);
