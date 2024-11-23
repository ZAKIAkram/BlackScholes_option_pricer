#pragma once
#include <iostream>
#include "Payoff.hpp"


class VanillaOption {
protected:
	// without reference, the compiler will refuse to instanciate an object from an abstract class
	// hence we should create the object first and then keep the reference here
	Payoff* _payoff; 
	double _maturity, _sigma;
	VanillaOption() = default;
	VanillaOption(const Payoff&, double, double);
	VanillaOption(const VanillaOption&);
	~VanillaOption();
public:
	virtual ~VanillaOption();
	//getters
	double getMaturity() const;
	void setMaturity(double);
	double computePayoff(double) const;






};