#pragma once

#include <functional>
#include <initializer_list>
#include <iostream>

#include "wav.hpp"
#include "utils.hpp"

class Output
{

private:

	double* x;

public:

	Output() : x(new double[GLOBAL_DURATION]) {}

	~Output() { delete[] x; }

	double& operator[](long unsigned i) const {
		return x[i];
	}

	// long unsigned size() const { return N; }

	Output operator+(const Output& a) {

		Output output;
		for (size_t i = 0; i < GLOBAL_DURATION; ++i) {
			output[i] = x[i] + a[i];
		}
		return output;
	}

	void play() {
		for (size_t i = 0; i < GLOBAL_DURATION; ++i) {
			int16_t sample = static_cast<int16_t>(x[i] * MAX_AMPLITUDE);
			writeAsBytes(std::cout, sample, 2); // left
			writeAsBytes(std::cout, sample, 2); // right
		}
	}

};

template <typename SynthFunction, typename ParamsType>
void play(SynthFunction synth, const ParamsType& params, double t) {
    long unsigned int dur = seconds(t);
    GLOBAL_DURATION = dur;
    Output toPlay = synth(params);
    toPlay.play();
}

