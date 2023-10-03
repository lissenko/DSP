#include <cmath>
#include <random>

#include "Signal.hpp"

double* Sine::generate(double A, double f, long unsigned N, double Ts) {
	double* x = new double[N];
	const double phaseInc = 2 * M_PI * f * 1.0 * Ts; // n == 1
	double phase = 0.0;
	for (long unsigned n = 0; n < N; ++n, phase += phaseInc) {
		x[n] = A * sin(phase);
	}
	return x;
}

double* Square::generate(double A, double f, long unsigned N, double Ts) {
	double* x = new double[N];
	const double phaseInc = 2 * M_PI * f * 1.0 * Ts; // n == 1
	double phase = 0.0;
	for (long unsigned n = 0; n < N; ++n, phase += phaseInc) {
		x[n] = (sin(phase) >= 0.0) ? A : -A;
	}
	return x;
}

double* SawTooth::generate(double A, double f, long unsigned N, double Ts) {
	double* x = new double[N];
	for (long unsigned n = 0; n < N; ++n) {
		double t = static_cast<double>(n) * Ts;
		// 2*A: multiply by two to double the scale (originally it is from 0 to 1)
		// such that it can be centered and be between -1 and 1
		// - A: center around 0
		x[n] = 2 * A * ((f * t) - std::floor(f * t)) - A;
	}
	return x;
}

double* Triangle::generate(double A, double f, long unsigned N, double Ts) {
	double* x = new double[N];
	for (long unsigned n = 0; n < N; ++n) {
		double t = static_cast<double>(n) * Ts;
		x[n] = 2 * A * std::abs(2 * (t*f - std::floor(t*f + 0.5))) - A;
	}
	return x;
}

double* ImpulseTrain::generate(double A, double f, long unsigned N, double Ts) {
	double* x = new double[N];
	long unsigned spike  = static_cast<long unsigned>(std::round( (1.0/Ts) / f) );
	for (long unsigned n = 0; n < N; ++n) {
		x[n] = (n % spike == 0) ? A : 0.0;
	}
	return x;
}

double* Noise::generate(const double A, const long unsigned N) {
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_real_distribution<double> distribution(-1.0, 1.0);
	double* x = new double[N];
	for (long unsigned n = 0; n < N; ++n) {
		x[n] = A * distribution(generator);
	}
	return x;
}
