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
	std::vector<double> _optionPrices;
	std::vector<double> _spot; // TODO: to remove

	Option() = default;
	Option(const std::shared_ptr<Payoff>&, double, double, int = 100);
	Option(const std::shared_ptr<Payoff>&, double, double, double, double, int = 100);
	Option(const Option&);
public:
	//virtual ~Option();
	//getters
	double getMaturity() const;
	virtual std::string getExerciseType() const = 0; // TODO
	Rates getRates() const;
	double getStrike() const;
	double getSigma() const;
	void setMaturity(double);
	double computePayoff(double) const;
	std::string getType() const;






};