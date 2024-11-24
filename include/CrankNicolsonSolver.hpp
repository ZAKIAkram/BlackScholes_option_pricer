#pragma once

#include <iostream>
#include <vector>

class CrankNicolsonSolver {
private:
	std::vector<double> _time_grid;
	std::vector<double> _spot_grid;
	std::vector<std::vector<double>> _grid;
	int _N, _M; // time, spot
	//double _S_max; // wait for strike price to use 3 * K or define it here ?
public:
	CrankNicolsonSolver() = default;
	CrankNicolsonSolver(int, int);

	std::vector<double> getTimeGrid() const;
	std::vector<double> getSpotGrid() const;
	int getN() const;
	int getM() const;

};
