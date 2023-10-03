#pragma once
#include "Signal.hpp"

class WaveTable {
private:
	Signal* signal = nullptr;
public:
	WaveTable() = default;
	WaveTable(Signal*);

	double* generate(double A, double f, long unsigned N, double Ts);

};
