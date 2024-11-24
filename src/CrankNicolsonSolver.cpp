#include "CrankNicolsonSolver.hpp"

CrankNicolsonSolver::CrankNicolsonSolver(int N, int M) : _N(N), _M(M), _time_grid(N), _spot_grid(M), _grid(N, std::vector<double>(M)) {}

std::vector<double> CrankNicolsonSolver::getTimeGrid() const {
	return _time_grid;
}

std::vector<double> CrankNicolsonSolver::getSpotGrid() const {
	return _spot_grid;
}

int CrankNicolsonSolver::getM() const {
	return _M;
}

int CrankNicolsonSolver::getN() const {
	return _N;
}
