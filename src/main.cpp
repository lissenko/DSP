#include <iostream>

#include "utils.hpp"
#include "wav.hpp"
#include "WavetableOscillator.hpp"
#include "config.hpp"
#include "Output.hpp"

#include <vector>

struct SynthParameters {
    double frequency = 330.0;
};

Output mySynth(const SynthParameters& params) {
    return ( Sine.get(params.frequency, 0.5)
           + Triangle.get(440.0, 0.1)
           + Triangle.get(880.0, 0.1)
           + Triangle.get(300.0 * 2, 0.1) );
}

int main() {
    SynthParameters params {};

	params.frequency = 500.0;
    play(mySynth, params, 1.0);

    free();
    return 0;
}

