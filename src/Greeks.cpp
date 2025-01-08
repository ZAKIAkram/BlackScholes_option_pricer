#include "Greeks.hpp"

Greeks::Greeks(int N, int M) : _M(M), _N(N), _delta(M + 1, 0.), _gamma(M + 1, 0.), _theta(M + 1, std::vector<double>(N + 1)), _rho(M + 1, 0.), _vega(M + 1, 0.) {}

Greeks::Greeks(const Option& option, const std::vector<std::vector<double>>& grid, int N, int M) : Greeks(N, M){

	double T = option.getMaturity();
	double K = option.getStrike();
	Rates rates = option.getRates();
	double sigma = option.getSigma();
	double S_max = 2. * K;

	double dt = T / static_cast<double>(_N);
	double ds = S_max / static_cast<double>(_M);


	for (int i = 1; i < _M; i++) {
		double S = i * ds;
		_delta[i] = (grid[i + 1][0] - grid[i - 1][0]) / (2 * ds);
		_gamma[i] = (grid[i + 1][0] - 2 * grid[i][0] + grid[i - 1][0]) / (ds * ds);
		_rho[i] = -(grid[i][0] - S * (grid[i + 1][0] - grid[i - 1][0]) / (2 * ds));
		_vega[i] = sigma * S * S * _gamma[i];
	}

	_delta[0] = (grid[1][0] - grid[0][0]) / ds;
	_delta[_M] = (grid[_M][0] - grid[M - 1][0]) / ds;
	_gamma[0] = 0.;
	_gamma[1] = 0.;
	_gamma[_M] = 0.;


	for (int t = 0; t < _N; t++) {
		for (int i = 0; i < _M + 1; i++) {
			_theta[i][t] = (grid[i][t + 1] - grid[i][t]) / dt;
		}
	}

	
	_vega[0] = _vega[1];
	_vega[M] = _vega[M - 1];

	
	_rho[0] = _rho[1];
	_rho[M] = _rho[_M - 1];
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
