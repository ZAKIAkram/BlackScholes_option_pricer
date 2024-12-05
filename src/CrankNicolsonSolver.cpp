#include "CrankNicolsonSolver.hpp"

CrankNicolsonSolver::CrankNicolsonSolver(size_t N, size_t M) : _N(N), _M(M), _time_grid(N), _spot_grid(M), _grid(M, std::vector<double>(N)) {
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

std::vector<std::vector<double>>& CrankNicolsonSolver::setupDirichletBoundaryConditions(const Option& option) const{
	std::vector<std::vector<double>> grid(_M, std::vector<double>(_N));
	double T = option.getMaturity();
	double K = option.getStrike();
	Rates r = option.getRates();
	double dt = 1 / static_cast<double>(_N);
	double ds = 1 / static_cast<double>(_M - 1);
	if (option.getType() == "call") {
		double S_max = 3 * K;
		for (int i = 0; i < _N; i++) {
			double r_t = r.at(i * dt);
			grid[0][i] = 0;
			grid[_M - 1][i] = S_max - K * exp(-r_t * T *(1 - i * dt));
		}
	}
	else {
		for (int i = 0; i < _N; i++) {
			double r_t = r.at(i * dt);
			grid[0][i] = K * exp(-r_t * T * (1 - i * dt));
			grid[_M - 1][i] = 0;
		}
	}
	return grid;
}

std::vector<std::vector<double>>& CrankNicolsonSolver::setupNeumanntBoundaryConditions(const Option& option, std::vector<std::vector<double>>& grid) const {
	for (int i = 0; i < _N; i++) {
		grid[0][i] = 2 * grid[1][i] - grid[2][i];
	}
	return grid;
}

std::vector<std::vector<double>> CrankNicolsonSolver::solve(const Option& option) const{
	std::vector<std::vector<double>> grid = setupDirichletBoundaryConditions(option);



}


