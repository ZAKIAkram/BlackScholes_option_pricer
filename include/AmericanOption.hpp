/**
 * @file AmericanOption.hpp
 * @brief Header file for the AmericanOption class, which represents an American option.
 */

#pragma once

#include "Option.hpp"

 /**
  * @class AmericanOption
  * @brief Classwhich represents an American option.
  * @inherits Option
  */
class AmericanOption : public Option{
public:
    /**
     * @brief constructor.
     * @param payoff Shared pointer to a Payoff object.
     * @param maturity Maturity of the option.
     * @param sigma Volatility of the option.
     * @param rate_0 Initial interest rate.
     * @param rate_1 Final interest rate.
     */
	AmericanOption(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma, double rate_0, double rate_1);

    /**
     * @brief Copy constructor.
     * @param other AmericanOption object to copy from.
     */

	AmericanOption(const AmericanOption& other);

    /**
     * @brief Get the exercise type of the option.
     * @return Exercise type as a string.
     */
	std::string getExerciseType() const override;
};
