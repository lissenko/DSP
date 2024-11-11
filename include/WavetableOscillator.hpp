#pragma once

#include <iostream>

#include "config.hpp"
#include "Output.hpp"
#include "wav.hpp"
#include "Signal.hpp"
#include "utils.hpp"

class WavetableOscillator {
private:

	Signal* wavetableGenerator;
	long unsigned tableSize = 0;
	const double* wavetable = wavetableGenerator->generate(1.0, 1.0, tableSize, static_cast<double>(tableSize));

	long double k_inc = 1;
	long double k = 0;

	long double get_k_inc(double freq) {
		return (freq * static_cast<double>(tableSize)) / SAMPLE_RATE;
	}

public:
	WavetableOscillator(Signal* waveType, double freq, long unsigned tableSize = 2048)
		: wavetableGenerator(waveType), tableSize(tableSize) {
			k_inc = get_k_inc(freq);
		}

	~WavetableOscillator() {
		delete[] wavetable;
	}

	WavetableOscillator(const WavetableOscillator&) = delete;
	void operator=(const WavetableOscillator&) = delete;

	const double* getTable() const { return wavetable; }

	void setFreq(const double freq) {
		k_inc = get_k_inc(freq);
	}

	double getNextSample() {
		/* Weighted average between the sample at the current index and the next index in the wavetable */
		k += k_inc;
		if (k >= tableSize) {
			k -= tableSize;
		}
		long unsigned idx1 = static_cast<long unsigned>(k);
		long unsigned idx2 = (idx1 + 1) % tableSize;
		// the fractional part of the phase
		double frac = k - static_cast<double>(idx1);
		return (1.0 - frac) * wavetable[idx1] + frac * wavetable[idx2];
	}


	void play(const double freq, const double A, const double t) {
		long unsigned N = seconds(t);
		double sample = 0.0;
		setFreq(freq);
		for (size_t i = 0; i < N; ++i) {
			sample = getNextSample() * A;
			writeAsBytes(std::cout, static_cast<int16_t>(sample * MAX_AMPLITUDE), 2);
			writeAsBytes(std::cout, static_cast<int16_t>(sample * MAX_AMPLITUDE), 2);
		}
	}

	void playNextSample() {
		double sample = getNextSample();
		writeAsBytes(std::cout, static_cast<int16_t>(sample * MAX_AMPLITUDE), 2);
		writeAsBytes(std::cout, static_cast<int16_t>(sample * MAX_AMPLITUDE), 2);
	}


	Output get(const double freq, const double A) {
		Output output;
		double sample = 0.0;
		setFreq(freq);
		for (size_t i = 0; i < GLOBAL_DURATION; ++i) {
			sample = getNextSample() * A;
			output[i] = sample;
		}
		return output;
	}


	void rest() {
		writeAsBytes(std::cout, static_cast<int16_t>(0.0), 2);
		writeAsBytes(std::cout, static_cast<int16_t>(0.0), 2);
	}

};

extern WavetableOscillator Sine;
extern WavetableOscillator Saw;
extern WavetableOscillator Square;
extern WavetableOscillator Triangle;
extern WavetableOscillator ImpulseTrain;
