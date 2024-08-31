#pragma once

#include "config.hpp"

class Signal {

private:

public:
	Signal() {};
	virtual ~Signal() = default;
	virtual double* generate(double, double, long unsigned, double Fs = SAMPLE_RATE) = 0;

};

class Sine : public Signal {
public:
	virtual double* generate(double, double, long unsigned, double Fs = SAMPLE_RATE) override;
};

class Square : public Signal {
public:
	virtual double* generate(double, double, long unsigned, double Fs = SAMPLE_RATE) override;
};

class Saw : public Signal {
public:
	virtual double* generate(double, double, long unsigned, double Fs = SAMPLE_RATE) override;
};

class Triangle : public Signal {
public:
	virtual double* generate(double, double, long unsigned, double Fs = SAMPLE_RATE) override;
};

class ImpulseTrain : public Signal {
public:
	virtual double* generate(double, double, long unsigned, double Fs = SAMPLE_RATE) override;
};

class Noise {
public:
	double* generate(const double, const long unsigned);
};


extern Signal* SINE_GENERATOR;
extern Signal* SAW_GENERATOR;
extern Signal* SQUARE_GENERATOR;
extern Signal* TRIANGLE_GENERATOR;
extern Signal* IMPULSE_TRAIN_GENERATOR;

void free();
