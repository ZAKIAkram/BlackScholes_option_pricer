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

std::vector<std::vector<double>>& CrankNicolsonSolver::setupDirichletBoundaryConditions(const std::shared_ptr<Option>& option) { // add const after
	//std::vector<std::vector<double>> grid(_M, std::vector<double>(_N));
	double T = option->getMaturity();
	double K = option->getStrike();
	Rates r = option->getRates();
	double dt = 1 / static_cast<double>(_N - 1);
	double ds = 1 / static_cast<double>(_M - 1);
	double S_max = 2.0 * K;
	if (option->getType() == "call") {
		double S_max = 3 * K;
		for (int i = 0; i < _N; i++) {
			double r_t = r.at(i * dt);
			_grid[0][i] = 0;
			_grid[_M - 1][i] = S_max - K * exp(-r_t * T *(1 - i * dt));
		}
	}
	else {
		for (int i = 0; i < _N; i++) {
			double r_t = r.at(i * dt);
			_grid[0][i] = K * exp(-r_t * T * (1 - i * dt));
			_grid[_M - 1][i] = 0;
		}
	}
	for (int i = 0; i < _M; i++) {
		double spot = i * ds * S_max;
		_grid[i][_N - 1] = option->computePayoff(spot);
	}
	return _grid;
}

std::vector<std::vector<double>>& CrankNicolsonSolver::setupNeumanntBoundaryConditions(const std::shared_ptr<Option>& option, std::vector<std::vector<double>>& grid) const {
	for (int i = 0; i < _N; i++) {
		grid[0][i] = 2 * grid[1][i] - grid[2][i];
	}
	return grid;
}

std::vector<std::vector<double>> CrankNicolsonSolver::solve(const std::shared_ptr<Option>& option){
	std::vector<std::vector<double>> grid = setupDirichletBoundaryConditions(option);
	double K = option->getStrike();
	double ds = 2.0 * K / static_cast<double>(_M - 1);
	/*for (size_t j = 0; j < _M; ++j) {
		double S = j * ds;
		_grid[j][0] = option->computePayoff(S);
	}*/
	for (size_t i = _N - 2; i > 0; --i) {
		solveTridiagonalSystem(_grid, i, option);
	}
	return _grid;
}


//std::vector<std::vector<double>>& CrankNicolsonSolver::solveTridiagonalSystem(std::vector<std::vector<double>>& grid, size_t index, const std::shared_ptr<Option>& option) {
//	// N = _M
//	// M = _N
//	// i for time
//	double T = option->getMaturity();
//	double K = option->getStrike();
//	Rates r = option->getRates();
//	double sigma = option->getSigma();
//	double dt = T / static_cast<double>(_N - 1);
//	double ds = 3.0 * K / static_cast<double>(_M - 1);
//	std::vector<double> lowerDiagonal(_M - 1), diagonal(_M), upperDiagonal(_M - 1);
//
//
//	for (int i = 0; i < _M - 1; i++) {
//		lowerDiagonal[i] = -.25 * dt * (sigma * sigma * i * i - r.at(index * dt / T) * i);
//		diagonal[i] = 1. + .5 * dt * (sigma * sigma * i * i + r.at(index * dt / T));
//		upperDiagonal[i] = -.25 * dt * (sigma * sigma * i * i + r.at(index * dt / T) * i);
//	}
//	// solving L * U = b
//	for (int i = 1; i < _M - 2; i++) {
//		double factor = lowerDiagonal[i] / diagonal[i - 1];
//		diagonal[i] -= factor * upperDiagonal[i - 1];
//		upperDiagonal[i - 1] -= factor * upperDiagonal[i - 1];
//	}
//
//	// backward substitution
//	for (size_t i = _M - 2; i > 0; i--) {
//
//		double value = (upperDiagonal[i - 1] - lowerDiagonal[i - 1] * _grid[i][index + 1]) / diagonal[i];
//		//_grid[index][i] = (upperDiagonal[i] - lowerDiagonal[i] * _grid[index + 1][i]) / diagonal[i];
//		_grid[i][index] = value;
//	}
//	return _grid;
//
//}


std::vector<std::vector<double>>& CrankNicolsonSolver::solveTridiagonalSystem(std::vector<std::vector<double>>& grid, int index, const std::shared_ptr<Option>& option) {
	double T = option->getMaturity();
	double K = option->getStrike();
	Rates r = option->getRates();
	double sigma = option->getSigma();
	double dt = T / static_cast<double>(_N - 1);
	double ds = 2.0 * K / static_cast<double>(_M - 1);

	std::vector<double> lowerDiagonal(_M), diagonal(_M), upperDiagonal(_M), b(_M);

	// Setup first point
	{
		double S0 = 0.0;
		double sigmaSq0 = sigma * sigma * S0 * S0;
		double r_t0 = r.at(index * dt);
		lowerDiagonal[0] = 0;  // No lower diagonal for first point
		diagonal[0] = 1.0 + 0.5 * dt * (sigmaSq0 + r_t0);
		upperDiagonal[0] = -0.25 * dt * (sigmaSq0 + r_t0);
		b[0] = _grid[0][index + 1];
	}

	// Setup middle points
	for (size_t j = 1; j < _M - 1; ++j) {
		double S = j * ds;
		double sigmaSq = sigma * sigma * S * S;
		double r_t = r.at(index * dt / T);

		lowerDiagonal[j] = -0.25 * dt * (sigmaSq - r_t * S / ds);
		diagonal[j] = 1.0 + 0.5 * dt * (sigmaSq + r_t);
		upperDiagonal[j] = -0.25 * dt * (sigmaSq + r_t * S / ds);

		// Finite difference approximation for the right-hand side
		b[j] = 0.5 * dt * (
			sigmaSq * (_grid[j + 1][index + 1] - 2.0 * _grid[j][index + 1] + _grid[j - 1][index + 1]) / (ds * ds) +
			r_t * S * (_grid[j + 1][index + 1] - _grid[j - 1][index + 1]) / (2.0 * ds)
			) + _grid[j][index + 1];
	}

	// Setup last point
	{
		double SLast = (_M - 1) * ds;
		double sigmaSqLast = sigma * sigma * SLast * SLast;
		double r_tLast = r.at(index * dt / T);

		lowerDiagonal[_M - 1] = -0.25 * dt * (sigmaSqLast - r_tLast * SLast / ds);
		diagonal[_M - 1] = 1.0 + 0.5 * dt * (sigmaSqLast + r_tLast);
		upperDiagonal[_M - 1] = 0;  // No upper diagonal for last point
		b[_M - 1] = _grid[_M - 1][index + 1];
	}

	// LU Decomposition
	std::vector<double> lu(_M);
	lu[0] = diagonal[0];

	for (size_t i = 1; i < _M; ++i) {
		double m = lowerDiagonal[i] / lu[i - 1];
		lu[i] = diagonal[i] - m * upperDiagonal[i - 1];
		b[i] -= m * b[i - 1];
	}

	// Back substitution
	_grid[_M - 1][index] = b[_M - 1] / lu[_M - 1];

	for (int i = _M - 2; i >= 0; --i) {
		_grid[i][index] = (b[i] - upperDiagonal[i] * _grid[i + 1][index]) / lu[i];
	}

	return _grid;
}
	
//##############################################################################################################
	// Initialize diagonal and off-diagonal vectors
	//std::vector<double> diagonal(_M - 2, 0.0);
	//std::vector<double> lowerDiagonal(_M - 3, 0.0);
	//std::vector<double> upperDiagonal(_M - 3, 0.0);
	//std::vector<double> b(_M - 2, 0.0);

	//// Fill coefficients
	//for (int i = 1; i < _M - 1; ++i) {
	//	double S_i = i * ds;
	//	double sigmaSq = sigma * sigma * S_i * S_i;
	//	double r_t = r.at(index * dt);

	//	if (i < _M - 2) {
	//		lowerDiagonal[i - 1] = -0.25 * dt * (sigmaSq - r_t * i);
	//	}
	//	diagonal[i - 1] = 1.0 + 0.5 * dt * (sigmaSq + r_t);
	//	if (i > 1) {
	//		upperDiagonal[i - 2] = -0.25 * dt * (sigmaSq + r_t * i);
	//	}

	//	// Fill b using Crank-Nicolson scheme
	//	b[i - 1] = 0.5 * dt * (sigmaSq * (_grid[i + 1][index + 1] - 2.0 * _grid[i + 1][index] + _grid[i - 1][index + 1]) +
	//		r_t * (_grid[i + 1][index + 1] - _grid[i - 1][index + 1]) / (2.0 * ds)) +
	//		_grid[i + 1][index];
	//}

	//// Forward elimination
	//for (int i = 1; i < _M - 2; ++i) {
	//	double factor = lowerDiagonal[i - 1] / diagonal[i - 1];
	//	diagonal[i] -= factor * upperDiagonal[i - 1];
	//	b[i] -= factor * b[i - 1];
	//}

	//// Backward substitution
	//std::vector<double> x(_M - 2, 0.0);
	//x[_M - 3] = b[_M - 3] / diagonal[_M - 3];
	//for (int i = _M - 4; i >= 0; --i) {
	//	x[i] = (b[i] - upperDiagonal[i] * x[i + 1]) / diagonal[i];
	//}

	//// Assign the solution back to the grid
	//for (int i = 1; i < _M - 1; ++i) {
	//	_grid[i][index] = x[i - 1];
	//}

	//return _grid;

//}



//std::vector<std::vector<double>>& CrankNicolsonSolver::solveTridiagonalSystem(
//	std::vector<std::vector<double>>& grid, size_t index, const std::shared_ptr<Option>& option) {
//
//	double T = option->getMaturity();
//	double K = option->getStrike();
//	Rates r = option->getRates();
//	double sigma = option->getSigma();
//	double dt = T / static_cast<double>(_N - 1);
//	double ds = K / static_cast<double>(_M - 1);
//
//	std::vector<double> lowerDiagonal(_M - 2), diagonal(_M - 2), upperDiagonal(_M - 2), b(_M - 2);
//
//	// Construct coefficients
//	for (int i = 1; i < _M - 1; i++) {
//		double spot = i * ds;
//		lowerDiagonal[i - 1] = -0.25 * dt * (sigma * sigma * spot * spot - r.at(index * dt) * spot);
//		diagonal[i - 1] = 1.0 + 0.5 * dt * (sigma * sigma * spot * spot + r.at(index * dt));
//		upperDiagonal[i - 1] = -0.25 * dt * (sigma * sigma * spot * spot + r.at(index * dt) * spot);
//
//		// Construct RHS vector (b)
//		b[i - 1] = 0.5 * dt * (sigma * sigma * spot * spot * _grid[index][i + 1]);
//	}
//
//	// Forward elimination
//	for (int i = 1; i < _M - 2; i++) {
//		double factor = lowerDiagonal[i] / diagonal[i - 1];
//		diagonal[i] -= factor * upperDiagonal[i - 1];
//		b[i] -= factor * b[i - 1];
//	}
//
//	// Backward substitution
//	for (int i = _M - 3; i >= 0; i--) {
//		if (i == _M - 3) {
//			_grid[index][i] = b[i] / diagonal[i];
//		}
//		else {
//			_grid[index][i] = (b[i] - upperDiagonal[i] * _grid[index][i + 2]) / diagonal[i];
//		}
//	}
//
//	return _grid;
//}