#include "CrankNicolsonSolver.hpp"

CrankNicolsonSolver::CrankNicolsonSolver(int N, int M) : _N(N), _M(M), _time_grid(N), _spot_grid(M), _grid(N, std::vector<double>(M)) {
	if (N <= 0) {
		throw std::invalid_argument("N should be greater than 0");
	}
	if (M <= 0) {
		throw std::invalid_argument("M should be greater than 0");
	}
	double dt = 1 / static_cast<double>(N); // T / N maybe ? multiply by T later if needed
	double ds = 1 / static_cast<double>(M);
	for (int i = 0; i < N; i++) {
		_time_grid[i] = i * dt;
	}

	for (int i = 0; i < M; i++) {
		_spot_grid[i] = i * ds;
	}
}

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

void CrankNicolsonSolver::setupDirichletBoundaryConditions(const std::shared_ptr<VanillaOption>& option){
	if (option->isCall()) {
		for (int i = 0; i < _N; i++) {
			_grid[i][0] = 0;
		}

	}
	
}
