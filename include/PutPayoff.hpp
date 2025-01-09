/**
 * @file PutPayoff.hpp
 * @brief Header file for the PutPayoff class, which represents the payoff of a put option and inherits fromPayoff class.
 */

#pragma once
#include "PayOff.hpp"

/**
 * @class PutPayoff
 * @brief Class representing the payoff of a put option.
 */
class PutPayoff : public Payoff {
public:
	PutPayoff() = default;
	/**
	 * @brief constructor.
	 * @param strike Strike price of the put option.
	 */
	PutPayoff(double strike);

	~PutPayoff() override = default;
	/**
	 * @brief Calculate the payoff for a given spot.
	 * @param spot Spot price of the underlying asset.
	 * @return Payoff value.
	 */
	double operator()(double spot) const override;

	/**
	 * @brief Get the type of the payoff, "call" or "put"
	 * @return type as a string.
	 */
	std::string getType() const override;

};
