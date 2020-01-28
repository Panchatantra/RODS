#pragma once

#include <armadillo>
#include "basis.h"

using namespace arma;

class timeseries : public basis
{
public:
	timeseries(const int id, const double dt, const vec &s);
	timeseries(const int id, const double dt, char* fileName);
	~timeseries();

	int nsteps;
	double dt;
	vec time, series;
};

