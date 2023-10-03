#pragma once

class Signal {

private:

public:
	Signal() {};
	virtual ~Signal() = default;
	virtual double* generate(double, double, long unsigned, double) = 0;

};

class Sine : public Signal {
public:
	virtual double* generate(double, double, long unsigned, double) override;
};

class Square : public Signal {
public:
	virtual double* generate(double, double, long unsigned, double) override;
};

class SawTooth : public Signal {
public:
	virtual double* generate(double, double, long unsigned, double) override;
};

class Triangle : public Signal {
public:
	virtual double* generate(double, double, long unsigned, double) override;
};

class ImpulseTrain : public Signal {
public:
	virtual double* generate(double, double, long unsigned, double) override;
};

class Noise {
public:
	double* generate(const double, const long unsigned);
};


