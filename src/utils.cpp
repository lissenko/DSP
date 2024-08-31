#include <iostream>

#include "utils.hpp"

#include "config.hpp"

void writeSamplesToFile(std::string filename, const double* x, long unsigned size) {
	std::ofstream myfile(filename);
	if (myfile.is_open()) {
		for (long unsigned count = 0; count < size; ++count) {
			myfile << x[count] << " " ;
		}
		myfile.close();
	}
	else std::cerr << "Unable to open file";
}

long unsigned sec(double t) {
	return static_cast<long unsigned>(t * SAMPLE_RATE);
}
