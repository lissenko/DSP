#include<cmath>
#include <iostream>

#include "WaveTable.hpp"

WaveTable::WaveTable(Signal* signal_p) : signal(signal_p) {}

double* WaveTable::generate(double A, double f, long unsigned N, double Ts) {
	double* x = new double[N];
	int L = static_cast<int>(std::round((1.0 / f) / Ts)); // #samples for one period
	double* table = signal->generate(A, f, static_cast<unsigned long>(L), Ts);
	int k = 0;
	for (long unsigned n = 0; n < N; ++n, ++k) {
		k %= L;
		x[n] = table[k];
	}
	delete[] table;
	return x;
}
