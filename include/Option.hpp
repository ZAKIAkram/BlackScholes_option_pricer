#pragma once

#include <iostream>
#include <memory>
#include "Rates.hpp"
#include "Payoff.hpp"


class Option {
protected:
	std::shared_ptr<Payoff> _payoff; 
	double _maturity, _sigma;
	Rates _rates;

	Option() = default;
	Option(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma);
	Option(const std::shared_ptr<Payoff>& payoff, double maturity, double sigma, double rate_0, double rate_1);
	Option(const Option& other);
public:
	//virtual ~Option();
	double getMaturity() const;
	virtual std::string getExerciseType() const = 0;
	virtual std::shared_ptr<Option> clone() const = 0;
	Rates getRates() const;
	double getStrike() const;
	double getSigma() const;
	void setMaturity(double);
	void setSigma(double);
	void setRates(Rates);
	double computePayoff(double) const;
	std::string getType() const;
};