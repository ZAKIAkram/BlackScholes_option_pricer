#pragma once
#include <iostream>
#include <memory>
#include "Rates.hpp"
#include "Payoff.hpp"


class VanillaOption {
protected:
	std::shared_ptr<Payoff> _payoff; 
	double _maturity, _sigma;
	Rates _rates;
	std::vector<double> _optionPrices;
	std::vector<double> _spot;

	VanillaOption() = default;
	VanillaOption(const std::shared_ptr<Payoff>&, double, double, int = 100);
	VanillaOption(const std::shared_ptr<Payoff>&, double, double, double, double, int = 100);
	VanillaOption(const VanillaOption&);
public:
	virtual ~VanillaOption();
	//getters
	double getMaturity() const;
	virtual std::string getType() const = 0; // TODO
	void setMaturity(double);
	double computePayoff(double) const;
	bool isCall() const;






};