#pragma once

#include <armadillo>

using namespace arma;

class timeseries
{
public:
	timeseries(const int id, const double dt, const vec &s);
	timeseries(const int id, const double dt, char* fileName);
	~timeseries();

	int id;
	int nsteps;
	double dt;
	vec time, series;
};

