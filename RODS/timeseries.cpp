#include "timeseries.h"

timeseries::timeseries(const int id, const double dt, const vec &s):
	basis(id), nsteps((int)s.n_rows), dt(dt),
    time(linspace(0.0, dt*(nsteps-1), nsteps)), series(vec(s))
{
}

timeseries::timeseries(const int id, const double dt, char* fileName):
	basis(id), dt(dt)
{
	series.load(fileName, raw_ascii);
	nsteps = (int)series.n_rows;
	time = linspace(0.0, dt*(nsteps-1), nsteps);
}

timeseries::~timeseries()
{
}
