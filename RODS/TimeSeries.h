#pragma once

#include <armadillo>
#include "Basis.h"

using namespace arma;

class TimeSeries : public Basis
{
public:
	TimeSeries(const int id, const double dt);
	TimeSeries(const int id, const double dt, const vec &s);
	TimeSeries(const int id, const double dt, char *fileName);
	~TimeSeries();

	void load(char *fileName);

	double dt;
	int nsteps;
	vec series;
};

