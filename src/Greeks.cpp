#include "Greeks.hpp"

Greeks::Greeks(int N, int M) : _M(M), _N(N), _delta(M, 0.), _gamma(M, 0.), _theta(M, std::vector<double>(N)), _rho(M, 0.), _vega(M, 0.) {}

Greeks::Greeks(const Option& option, const std::vector<std::vector<double>>& grid, int N, int M) : Greeks(N, M){

	double T = option.getMaturity();
	double K = option.getStrike();
	Rates rates = option.getRates();
	double sigma = option.getSigma();
	double S_max = 2. * K;

	double dt = T / static_cast<double>(_N - 1);
	double ds = S_max / static_cast<double>(_M - 1);


	for (int i = 1; i < _M - 1; ++i) {
		_delta[i] = (grid[i + 1][0] - grid[i - 1][0]) / (2 * ds);
		_gamma[i] = (grid[i + 1][0] - 2 * grid[i][0] + grid[i - 1][0]) / (ds * ds);
	}

	_delta[0] = (grid[1][0] - grid[0][0]) / ds;
	_delta[_M - 1] = (grid[M - 1][0] - grid[M - 2][0]) / ds;
	_gamma[0] = 0.0;
	_gamma[_M - 1] = 0.0;


	for (int t = 0; t < _N - 1; ++t) {
		for (int i = 0; i < _M; ++i) {
			_theta[i][t] = (grid[i][t] - grid[i][t + 1]) / dt;
		}
	}

	double epsilon = 0.01;
	for (int i = 0; i < _M; ++i) {
		auto option_up_sigma = option.clone();
		option_up_sigma->setSigma(option.getSigma() + epsilon);
		std::vector<std::vector<double>> grid_up_sigma = CrankNicolsonSolver::solve(*option_up_sigma, _N, _M);

		auto option_down_sigma = option.clone();
		option_down_sigma->setSigma(option.getSigma() - epsilon);
		std::vector<std::vector<double>> grid_down_sigma = CrankNicolsonSolver::solve(*option_down_sigma, _N, _M);

		_vega[i] = (grid_up_sigma[i][0] - grid_down_sigma[i][0]) / (2 * epsilon);

		auto option_up_r = option.clone();
		option_up_r->setRates(Rates(option.getRates(), epsilon));
		std::vector<std::vector<double>> grid_up_r = CrankNicolsonSolver::solve(*option_up_r, _N, _M);

		auto option_down_r = option.clone();
		option_up_r->setRates(Rates(option.getRates(), -epsilon));
		std::vector<std::vector<double>> grid_down_r = CrankNicolsonSolver::solve(*option_down_r, _N, _M);

		_rho[i] = (grid_up_r[i][0] - grid_down_r[i][0]) / (2 * epsilon);
	}

}

std::vector<double> Greeks::getDelta() const {
	return _delta;
}

std::vector<double> Greeks::getGamma() const {
	return _gamma;
}

std::vector<double> Greeks::getRho() const {
	return _rho;
}

std::vector<double> Greeks::getVega() const {
	return _vega;
}

std::vector<std::vector<double>> Greeks::getTheta() const {
	return _theta;
}
