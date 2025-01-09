/**
 * @file EuropeanOption.hpp
 * @brief Header file for the EuropeanOption class, which represents a European option.
 */

#pragma once
#include "Option.hpp"

 /**
  * @class EuropeanOption
  * @brief Class representing a European option.
  */
class EuropeanOption : public Option {
public:
    /**
     * @brief constructor for EuropeanOption.
     * @param option Shared pointer to a Payoff object.
     * @param maturity Maturity of the option.
     * @param sigma Volatility of the option.
     * @param rate_0 Initial interest rate.
     * @param rate_1 Final interest rate.
     */
	EuropeanOption(const std::shared_ptr<Payoff>& option, double maturity, double sigma, double rate_0, double rate_1);
    /**
     * @brief Copy constructor for EuropeanOption.
     * @param other Another EuropeanOption object to copy from.
     */
	EuropeanOption(const EuropeanOption& other);
	std::string getExerciseType() const override;
	//std::shared_ptr<Option> clone() const override;
};
