/**
 * @file CallPayoff.hpp
 * @brief Header file for the CallPayoff class, which represents the payoff of a call option.
 */
#pragma once

#include "Payoff.hpp"

 /**
  * @class CallPayoff
  * @brief Class representing the payoff of a call option.
  */
class CallPayoff : public Payoff {
public:
	CallPayoff() = default;
	/**
	 * @brief constructor.
	 * @param strike Strike price of the call option.
	 */
	CallPayoff(double strike);
	~CallPayoff() override = default;

	/**
	 * @brief Calculate the payoff for a given spot price.
	 * @param spot spot price of the underlying asset.
	 * @return Payoff.
	 */
	double operator()(double spot) const override;
	/**
	 * @brief Get the type of the payoff: "call" or "put".
	 * @return Type as a string.
	 */
	std::string getType() const override;

};
