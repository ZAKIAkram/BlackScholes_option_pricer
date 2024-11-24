#pragma once
#include <iostream>
#include <memory>
#include "Rates.hpp"
#include "Payoff.hpp"


class VanillaOption {
protected:
	// without reference, the compiler will refuse to instanciate an object from an abstract class
	// hence we should create the object first and then keep the reference here
	Payoff* _payoff; 
	double _maturity, _sigma;
	Rates _rates;
	VanillaOption() = default;
	VanillaOption(const Payoff&, double, double, int = 100);
	VanillaOption(const Payoff&, double, double, double, double, int = 100);
	VanillaOption(const VanillaOption&);
public:
	virtual ~VanillaOption();
	//getters
	double getMaturity() const;
	virtual std::string getType() const = 0; // TODO
	void setMaturity(double);
	double computePayoff(double) const;






};