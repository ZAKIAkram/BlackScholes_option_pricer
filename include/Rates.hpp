/**
 * @file Rates.hpp
 * @brief Header file for the Rates class, which represents interest rates from t = 0 tp maturity using linear interpolation.
 */
#pragma once

#include <iostream>

/**
 * @class Rates
 * @brief Class representing interest rates.
 */
class Rates {
private:
	double _rate_0, _rate_1;
public:
	Rates() = default;
	/**
	 * @brief constructor.
	 * @param rate_0 initial interest rate.
	 * @param rate_1 final interest rate.
	 */
	Rates(double rate_0, double rate_1); 

	/**
	 * @brief copy constructor.
	 * @param other Another Rates objetc to copy.
	 */
	Rates(const Rates& other);
	~Rates() = default;

	/**
	 * @brief get the interest rate at a specific time using linear interpolation.
	 * @param t Time at which to get the interest rate.
	 * @return interest rate at time t.
	 */
	double at(double t) const;
};
