/**
 * @file Option.hpp
 * @brief Header file for the Option class.
 */

#pragma once

#include <iostream>
#include <memory>
#include "Rates.hpp"
#include "Payoff.hpp"


/**
 * @class Option
 * @brief Abstract class options.
 */
class Option {
protected:
	std::shared_ptr<Payoff> _payoff; 
	double _maturity, _sigma;
	Rates _rates; ///< Interest rates associated with the option.

	Option() = default;

	/**
	 * @brief constructor, made protected so we cannot instanciate an option object
	 * @param payoff Shared pointer to a Payoff object.
	 * @param maturity Maturity of the option.
	 * @param sigma Volatility of the option.
	 */
	Option(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma);

	/**
	 * @brief constructor with rates.
	 * @param payoff Shared pointer to a Payoff object.
	 * @param maturity Maturity of the option.
	 * @param sigma Volatility of the option.
	 * @param rate_0 Initial interest rate, ie at t = 0.
	 * @param rate_1 Final interest rate, ie at t = maturity.
	 */
	Option(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma, double rate_0, double rate_1);

	/**
	 * @brief Copy constructor.
	 * @param other Option object to copy from.
	 */
	Option(const Option& other);
public:
	virtual ~Option() = default;
	double getMaturity() const;
	virtual std::string getExerciseType() const = 0;
	Rates getRates() const;
	double getStrike() const;
	double getSigma() const;
	void setMaturity(double);
	void setSigma(double);
	void setRates(Rates);

	/**
	 * @brief Compute the payoff of the option.
	 * @param spot Price of the underlying asset.
	 * @return Payoff value.
	 */
	double computePayoff(double) const;
	std::string getType() const;
};