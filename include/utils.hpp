#pragma once

#include <string>
#include <fstream>

void writeSamplesToFile(std::string filename, const double*, long unsigned);

long unsigned sec(double t);
