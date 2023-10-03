#include <iostream>

#include "utils.hpp"
#include "wav.hpp"
#include "Signal.hpp"
#include "WaveTable.hpp"

int main() {

	double A = 0.16;
	double f = 440.0;
	int Fs = 48000;
	double t = 5.0;
	double Ts = 1.0/ static_cast<double>(Fs);
	long unsigned N = static_cast<long unsigned>(t / Ts);

	Signal* signal = new Sine();
	WaveTable waveTable(signal);
	double* x = waveTable.generate(A, f, N, Ts);
	createWavFile("sound.wav", x, Fs, N);
	delete[] x;
	delete signal;

	return 0;
}
