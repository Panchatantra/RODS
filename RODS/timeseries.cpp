#include "timeseries.h"

timeseries::timeseries(const int id, const double dt, const vec &s):
	basis(id), dt(dt), series(vec(s))
{
	this->nsteps = (int)series.n_rows;
	time = linspace(0.0, dt*(this->nsteps - 1), this->nsteps);
}

timeseries::timeseries(const int id, const double dt, char* fileName):
	basis(id), dt(dt)
{
	series.load(fileName, raw_ascii);
	this->nsteps = (int)series.n_rows;
	time = linspace(0.0, dt*(this->nsteps-1), this->nsteps);
}

timeseries::~timeseries()
{
}
