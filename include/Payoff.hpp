/**
 * @file Payoff.hpp
 * @brief Header file for the Payoff class, which represents an abstract class for option payoffs.
 */
#pragma once
#include <iostream>

/**
 * @class Payoff
 * @brief Abstract class for option payoffs.
 * @inheritDoc
 */
class Payoff {
protected:
	double _strike; ///< Strike price of the option.
	Payoff() = default;

	/**
	 * @brief constructor.
	 * @param strike Strike price of the option.
	 */
	Payoff(double strike);
public:

	double getStrike() const;

	/**
	 * @brief Calculate the payoff for a given spot price.
	 * @param spot Spot price of the underlying asset.
	 * @return Payoff value.
	 */
	virtual double operator()(double spot) const = 0;
	virtual ~Payoff() = default;
	virtual std::string getType() const = 0;
};
