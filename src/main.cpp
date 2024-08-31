#include <iostream>

#include "utils.hpp"
#include "wav.hpp"
#include "WavetableOscillator.hpp"
#include "config.hpp"

int main() {

	WavetableOscillator& osc = Saw;

	double freq = 0.0;
	osc.setFreq(freq);

	for (int i = 0; i < 10000000000; ++i) {
		if (i % 10000 == 0) {
			freq += 100.0;
			osc.setFreq(freq);
		}
		osc.playNextSample();
	}

    free();
    return 0;
}

