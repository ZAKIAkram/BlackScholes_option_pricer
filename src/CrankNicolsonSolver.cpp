#include "CrankNicolsonSolver.hpp"

std::vector<std::vector<double>> CrankNicolsonSolver::solve(const Option& option, int N, int M) {
	double T = option.getMaturity();
	double K = option.getStrike();
	Rates rates = option.getRates();
	double sigma = option.getSigma();
	double S_max = 2. * K;
	double dt = T / static_cast<double>(N);
	double ds = S_max / static_cast<double>(M);
	std::string optionType = option.getType();

	std::vector<double> a(M - 1, 0.0);
	std::vector<double> b(M - 1, 0.0);
	std::vector<double> c(M - 1, 0.0);
	std::vector<double> d(M - 1, 0.0);
	std::vector<std::vector<double>> grid(N + 1, std::vector<double>(M + 1, 0.));

	setupBoundaryConditions(option, grid);

	for (int t = N - 1; t >= 0; --t) {
		double r = rates.at(t * dt / T);
		//Set up the tridiagonal system
		for (int i = 1; i < M; ++i) {
			double stockPrice = i * ds;
			double alpha = 0.25 * dt * (sigma * sigma * i * i - r * i);
			double beta = -0.5 * dt * (sigma * sigma * i * i + r);
			double gamma = 0.25 * dt * (sigma * sigma * i * i + r * i);

			a[i - 1] = -alpha;
			b[i - 1] = 1 - beta;
			c[i - 1] = -gamma;
			d[i - 1] = alpha * grid[t + 1][i - 1] + (1 + beta) * grid[t + 1][i] + gamma * grid[t + 1][i + 1];
		}

		// Thomas algorithm
		for (int i = 1; i < M - 1; ++i) {
			double m = a[i] / b[i - 1];
			b[i] -= m * c[i - 1];
			d[i] -= m * d[i - 1];
		}

		grid[t][M - 1] = d[M - 2] / b[M - 2];
		for (int i = M - 2; i > 0; --i) {
			grid[t][i] = (d[i - 1] - c[i - 1] * grid[t][i + 1]) / b[i - 1];
		}

		if (option.getExerciseType() == "American") {
			earlyExercise(option, grid, t);
		}
	}
	return grid;
}

void CrankNicolsonSolver::earlyExercise(const Option& option, std::vector<std::vector<double>>& grid, int t){
	double K = option.getStrike();
	int M = grid[0].size() - 1;
	double ds = 2. * K / static_cast<double>(M);
	for (int i = 1; i < M; ++i) {
		double spot = i * ds;
		grid[t][i] = std::max(grid[t][i], option.computePayoff(spot));
	}
}

void CrankNicolsonSolver::setupBoundaryConditions(const Option& option, std::vector<std::vector<double>>& grid){
	double T = option.getMaturity();
	double K = option.getStrike();
	Rates rates = option.getRates();
	double sigma = option.getSigma();
	int N = grid.size() - 1;
	int M = grid[0].size() - 1;
	double dt = T / static_cast<double>(N);
	double ds = 2. * K / static_cast<double>(M);
	std::string optionType = option.getType();
	double S_max = 2. * K;
	for (int t = N - 1; t >= 0; --t) {
		double r = rates.at(t * dt / T);
		if (optionType == "call") {
			grid[t][0] = 0.;
			grid[t][M] = (S_max - K * exp(-r * (T - t * dt)));
		}
		else {
			grid[t][0] = K * exp(-r * (T - t * dt));
			grid[t][M] = 0.;
		}
	}
	for (int i = 0; i <= M; ++i) {
		double spot = i * ds;
		grid[N][i] = option.computePayoff(spot);
	}
}
