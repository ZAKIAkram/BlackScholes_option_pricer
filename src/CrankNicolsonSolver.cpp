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

//std::vector<std::vector<double>>& CrankNicolsonSolver::setupDirichletBoundaryConditions(const std::shared_ptr<Option>& option) { // add const after
//	//std::vector<std::vector<double>> grid(_M, std::vector<double>(_N));
//	double T = option->getMaturity();
//	double K = option->getStrike();
//	Rates r = option->getRates();
//	double dt = 1 / static_cast<double>(_N - 1);
//	double ds = 1 / static_cast<double>(_M - 1);
//	double S_max = 2.0 * K;
//	if (option->getType() == "call") {
//		for (int i = 0; i < _N; i++) {
//			double r_t = r.at(i * dt);
//			_grid[0][i] = 0;
//			_grid[_M - 1][i] = S_max - K * exp(-r_t * T *(1 - i * dt));
//		}
//	}
//	else {
//		for (int i = 0; i < _N; i++) {
//			double r_t = r.at(i * dt);
//			_grid[0][i] = K * exp(-r_t * T * (1 - i * dt));
//			_grid[_M - 1][i] = 0;
//		}
//	}
//	for (int i = 0; i < _M; i++) {
//		double spot = i * ds * S_max;
//		_grid[i][_N - 1] = option->computePayoff(spot);
//	}
//	return _grid;
//}
//
//std::vector<std::vector<double>>& CrankNicolsonSolver::setupNeumanntBoundaryConditions(const std::shared_ptr<Option>& option, std::vector<std::vector<double>>& grid) const {
//	for (int i = 0; i < _N; i++) {
//		grid[0][i] = 2 * grid[1][i] - grid[2][i];
//	}
//	return grid;
//}


std::vector<std::vector<double>>& CrankNicolsonSolver::setupDirichletBoundaryConditions(const std::shared_ptr<Option>& option) {
	// Parameters
	double T = option->getMaturity();
	double K = option->getStrike();
	Rates r = option->getRates();
	double dt = T / static_cast<double>(_N - 1);
	double S_max = 2.0 * K;

	// Adjust S_max for call options
	if (option->getType() == "call") {
		S_max = 3.0 * K;
	}

	// Stock price step size
	double ds = S_max / static_cast<double>(_M - 1);

	// Boundary conditions
	for (int j = 0; j < _N; j++) { // Iterate over time steps
		double t = j * dt;         // Elapsed time
		double r_t = r.at(t);      // Interest rate at time t

		// Left boundary: S = 0
		if (option->getType() == "call") {
			_grid[0][j] = 0.0; // Call option: intrinsic value at S=0 is 0
		}
		else {
			_grid[0][j] = K * exp(-r_t * (T - t)); // Put option: discounted strike price
		}

		// Right boundary: S = S_max
		if (option->getType() == "call") {
			_grid[_M - 1][j] = S_max - K * exp(-r_t * (T - t)); // Call option: intrinsic value at S_max
		}
		else {
			_grid[_M - 1][j] = 0.0; // Put option: intrinsic value at S_max is 0
		}
	}

	// Final payoff at maturity (t = T)
	for (int i = 0; i < _M; i++) { // Iterate over stock prices
		double S = i * ds;         // Stock price
		_grid[i][_N - 1] = option->computePayoff(S); // Payoff at maturity
	}

	return _grid;
}


std::vector<std::vector<double>> CrankNicolsonSolver::solve(const std::shared_ptr<Option>& option){
	std::vector<std::vector<double>> grid = setupDirichletBoundaryConditions(option);
	double K = option->getStrike();
	double ds = 2.0 * K / static_cast<double>(_M - 1);
	for (int i = _N - 2; i >= 0; --i) {
		solveTridiagonalSystem(i, option);
	}
	return _grid;
}

std::vector<std::vector<double>> CrankNicolsonSolver::solve_2(const std::shared_ptr<Option>& option) {
	double T = option->getMaturity();
	double K = option->getStrike();
	Rates rates = option->getRates();
	double sigma = option->getSigma();
	double S_max = 2. * K;
	double dt = T / static_cast<double>(_N);
	double ds = S_max / static_cast<double>(_M);
	std::string optionType = option->getType();
	bool isCall = (optionType == "call");
	std::vector<double> a(_M - 1, 0.0);
	std::vector<double> b(_M - 1, 0.0);
	std::vector<double> c(_M - 1, 0.0);
	std::vector<double> d(_M - 1, 0.0);
	std::vector<std::vector<double>> grid(_N + 1, std::vector<double>(_M + 1, 0.));

	// Grid initialization
	std::vector<double> V(_M + 1, 0.0);    // Current time step
	std::vector<double> V_prev(_M + 1, 0.0); // Previous time step

	// Set up boundary conditions at maturity
	for (int i = 0; i <= _M; ++i) {
		double spot = i * ds;
		V[i] = option->computePayoff(spot);
	}

	// Time-stepping loop
	for (int n = _N - 1; n >= 0; --n) {
		// Copy current values to the previous time step
		V_prev = V;
		double r = rates.at(n * dt / T);
		// Set up the tridiagonal system
		for (int i = 1; i < _M; ++i) {
			double stockPrice = i * ds;
			double alpha = 0.25 * dt * (sigma * sigma * i * i - r * i);
			double beta = -0.5 * dt * (sigma * sigma * i * i + r);
			double gamma = 0.25 * dt * (sigma * sigma * i * i + r * i);

			a[i - 1] = -alpha;
			b[i - 1] = 1 - beta;
			c[i - 1] = -gamma;
			d[i - 1] = alpha * V_prev[i - 1] + (1 + beta) * V_prev[i] + gamma * V_prev[i + 1];
		}

		// Thomas algorithm for solving the tridiagonal system
		for (int i = 1; i < _M - 1; ++i) {
			double m = a[i] / b[i - 1];
			b[i] -= m * c[i - 1];
			d[i] -= m * d[i - 1];
		}

		V[_M - 1] = d[_M - 2] / b[_M - 2];
		for (int i = _M - 2; i > 0; --i) {
			V[i] = (d[i - 1] - c[i - 1] * V[i + 1]) / b[i - 1];
		}

		// Boundary conditions
		V[0] = isCall ? 0.0 : K * exp(-r * (T - n * dt));
		V[_M] = isCall ? (S_max - K * exp(-r * (T - n * dt))) : 0.0;

		// Early exercise boundary condition for American options
		if (option->getExerciseType() == "American") {
			for (int i = 1; i < _M; ++i) {
				double spot = i * ds;
				double intrinsicValue = option->computePayoff(spot);
				V[i] = std::max(V[i], intrinsicValue);
			}
		}
		grid[n] = V;
	}
	return grid;
}



//std::vector<std::vector<double>>& CrankNicolsonSolver::solveTridiagonalSystem(std::vector<std::vector<double>>& grid, int index, const std::shared_ptr<Option>& option) {
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


//std::vector<std::vector<double>>& CrankNicolsonSolver::solveTridiagonalSystem(int index, const std::shared_ptr<Option>& option) {
//	double T = option->getMaturity();
//	double K = option->getStrike();
//	Rates r = option->getRates();
//	double sigma = option->getSigma();
//	double dt = T / static_cast<double>(_N - 1);
//	double ds = 2.0 * K / static_cast<double>(_M - 1);
//
//	std::vector<double> lowerDiagonal(_M), diagonal(_M), upperDiagonal(_M), b(_M);
//
//	// Setup first point
//	{
//		double S0 = 0.0;
//		double sigmaSq0 = sigma * sigma * S0 * S0;
//		double r_t0 = r.at(index * dt / T);
//		lowerDiagonal[0] = 0;  // No lower diagonal for first point
//		diagonal[0] = 1.0 + 0.5 * dt * (sigmaSq0 + r_t0);
//		upperDiagonal[0] = -0.25 * dt * (sigmaSq0 + r_t0);
//		b[0] = _grid[0][index + 1];
//	}
//
//	// Setup middle points
//	for (int j = 1; j < _M - 1; ++j) {
//		double S = j * ds;
//		double sigmaSq = sigma * sigma * S * S;
//		double r_t = r.at(index * dt / T);
//
//		lowerDiagonal[j] = -0.25 * dt * (sigmaSq - r_t * S / ds);
//		diagonal[j] = 1.0 + 0.5 * dt * (sigmaSq + r_t);
//		upperDiagonal[j] = -0.25 * dt * (sigmaSq + r_t * S / ds);
//
//		// Finite difference approximation for the right-hand side
//		b[j] = 0.5 * dt * (
//			sigmaSq * (_grid[j + 1][index + 1] - 2.0 * _grid[j][index + 1] + _grid[j - 1][index + 1]) / (ds * ds) +
//			r_t * S * (_grid[j + 1][index + 1] - _grid[j - 1][index + 1]) / (2.0 * ds)
//			) + _grid[j][index + 1];
//	}
//
//	// Setup last point
//	{
//		double SLast = (_M - 1) * ds;
//		double sigmaSqLast = sigma * sigma * SLast * SLast;
//		double r_tLast = r.at(index * dt / T);
//
//		lowerDiagonal[_M - 1] = -0.25 * dt * (sigmaSqLast - r_tLast * SLast / ds);
//		diagonal[_M - 1] = 1.0 + 0.5 * dt * (sigmaSqLast + r_tLast);
//		upperDiagonal[_M - 1] = 0;  // No upper diagonal for last point
//		b[_M - 1] = _grid[_M - 1][index + 1];
//	}
//
//	// LU Decomposition
//	std::vector<double> lu(_M);
//	lu[0] = diagonal[0];
//
//	for (int i = 1; i < _M; ++i) {
//		double m = lowerDiagonal[i] / lu[i - 1];
//		lu[i] = diagonal[i] - m * upperDiagonal[i - 1];
//		b[i] -= m * b[i - 1];
//	}
//
//	// Back substitution
//	_grid[_M - 1][index] = b[_M - 1] / lu[_M - 1];
//
//	for (int i = _M - 2; i >= 0; --i) {
//		_grid[i][index] = (b[i] - upperDiagonal[i] * _grid[i + 1][index]) / lu[i];
//	}
//
//	return _grid;
//}


std::vector<std::vector<double>>& CrankNicolsonSolver::solveTridiagonalSystem(int timeIndex, const std::shared_ptr<Option>& option) {
	// Parameters from the option and solver
	double T = option->getMaturity();
	double K = option->getStrike();
	Rates rates = option->getRates();
	double sigma = option->getSigma();
	double dt = T / static_cast<double>(_N - 1);
	double ds = 2.0 * K / static_cast<double>(_M - 1);
	double r = rates.at(timeIndex * dt / T); // Rate at current time index

	// Coefficients for the tridiagonal matrix
	std::vector<double> a(_M - 2, 0.0); // Lower diagonal
	std::vector<double> b(_M - 2, 0.0); // Main diagonal
	std::vector<double> c(_M - 2, 0.0); // Upper diagonal
	std::vector<double> d(_M - 2, 0.0); // Right-hand side

	// Fill the tridiagonal system
	for (int i = 1; i < _M - 1; ++i) {
		double Si = i * ds;
		double alpha = 0.25 * dt * (sigma * sigma * i * i - r * i);
		double beta = -0.5 * dt * (sigma * sigma * i * i + r);
		double gamma = 0.25 * dt * (sigma * sigma * i * i + r);

		a[i - 1] = -alpha;
		b[i - 1] = 1.0 - beta;
		c[i - 1] = -gamma;

		// RHS using values from the next time step
		d[i - 1] = alpha * _grid[i - 1][timeIndex + 1] +
			(1.0 + beta) * _grid[i][timeIndex + 1] +
			gamma * _grid[i + 1][timeIndex + 1];
	}

	// Thomas algorithm for solving tridiagonal system
	for (int i = 1; i < _M - 2; ++i) {
		double m = a[i] / b[i - 1];
		b[i] -= m * c[i - 1];
		d[i] -= m * d[i - 1];
	}

	// Back substitution
	std::vector<double> V(_M - 2, 0.0);
	V[_M - 3] = d[_M - 3] / b[_M - 3];
	for (int i = _M - 4; i >= 0; --i) {
		V[i] = (d[i] - c[i] * V[i + 1]) / b[i];
	}

	// Update the grid for the current time step
	for (int i = 1; i < _M - 1; ++i) {
		_grid[i][timeIndex] = V[i - 1];
	}

	// Return the updated grid reference
	return _grid;
}






//void croutSolver(const std::vector<std::vector<double>>& A, const std::vector<double>& b, std::vector<double>& x) {
//	int n = A.size();
//	std::vector<double> z(n, 0.0);
//	std::vector<std::vector<double>> L(n, std::vector<double>(n, 0.0));
//	std::vector<std::vector<double>> U(n, std::vector<double>(n, 0.0));
//
//	// Factorize A = LU
//	L[0][0] = A[0][0];
//	U[0][1] = A[0][1] / L[0][0];
//	for (int i = 1; i < n - 1; ++i) {
//		L[i][i - 1] = A[i][i - 1];
//		L[i][i] = A[i][i] - L[i][i - 1] * U[i - 1][i];
//		U[i][i + 1] = A[i][i + 1] / L[i][i];
//	}
//	L[n - 1][n - 2] = A[n - 1][n - 2];
//	L[n - 1][n - 1] = A[n - 1][n - 1] - L[n - 1][n - 2] * U[n - 2][n - 1];
//
//	// Forward substitution for Lz = b
//	z[0] = b[0] / L[0][0];
//	for (int i = 1; i < n; ++i) {
//		z[i] = (b[i] - L[i][i - 1] * z[i - 1]) / L[i][i];
//	}
//
//	// Backward substitution for Ux = z
//	x[n - 1] = z[n - 1];
//	for (int i = n - 2; i >= 0; --i) {
//		x[i] = z[i] - U[i][i + 1] * x[i + 1];
//	}
//}
//
//void matrixVector(const std::vector<std::vector<double>>& T, const std::vector<double>& V, const std::vector<double>& k, std::vector<double>& result) {
//	int n = T.size();
//	for (int i = 0; i < n; ++i) {
//		result[i] = k[i];
//		for (int j = 0; j < n; ++j) {
//			result[i] += T[i][j] * V[j];
//		}
//	}
//}
//
//
//std::vector<std::vector<double>> CrankNicolsonSolver::solve_2(const std::shared_ptr<Option>& option) {
//	double T = option->getMaturity();
//	double K = option->getStrike();
//	Rates rates = option->getRates();
//	double sigma = option->getSigma();
//	double dt = T / static_cast<double>(_N - 1);
//	double ds = 2.0 * K / static_cast<double>(_M - 1);
//
//	std::vector<double> a(_M - 1), b(_M - 1), c(_M - 1), d(_M - 1);
//	for (int j = 0; j < _M - 1; j++) {
//		double r = rates.at(j * dt / T);
//		a[j] = .25 * (j + 1) * dt * (sigma * sigma * (j + 1) - r);
//		b[j] = (1 - .5 * sigma * sigma * (j + 1) * (j + 1) * dt);
//		c[j] = .25 * (j + 1) * dt * (sigma * sigma * (j + 1) + r);
//		d[j] = (1 + (r + .5 * sigma * sigma * (j + 1) * (j + 1)) * dt);
//	}
//	std::vector<std::vector<double>> T1(_M - 1, std::vector<double>(_M - 1, 0.)), T2(_M - 1, std::vector<double>(_M - 1, 0.));
//	for (int j = 0; j < _M - 1; j++) {
//		T1[j][j] = b[j];
//		T2[j][j] = d[j];
//	}
//
//	for (int j = 0; j < _M - 2; j++) {
//		T1[j][j + 1] = c[j];
//		T1[j + 1][j] = a[j + 1];
//		T2[j][j + 1] = -c[j];
//		T2[j + 1][j] = -a[j + 1];
//	}
//
//	std::vector<double> V(_M - 1, 0.), k(_M - 1, 0.);
//	k[0] = a[0] * (2.0 * K);
//	for (int j = 0; j < _M - 1; ++j) {
//		V[j] = std::max(K - ds * (j + 1), 0.0); // Put option payoff
//	}
//
//	// Time-stepping
//	std::vector<double> vec1(_M - 1, 0.0);
//	for (int n = _N - 1; n > 0; --n) {
//		// Compute T1 * V + k
//		matrixVector(T1, V, k, vec1);
//
//		// Solve T2 * V_new = vec1 using Crout's method
//		croutSolver(T2, vec1, V);
//
//		// Apply early exercise condition (American option)
//		for (int j = 0; j < _M - 1; ++j) {
//			V[j] = std::max(V[j], K - ds * (j + 1));
//		}
//	}
//
//	// Collect results into a table for output
//	std::vector<std::vector<double>> result(_N, std::vector<double>(_M, 0.0));
//	for (int j = 0; j < _M; ++j) {
//		result[0][j] = std::max(K - ds * j, 0.0); // Initial values
//	}
//	return result;
//
//}
