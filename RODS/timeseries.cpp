#include "timeseries.h"

timeseries::timeseries(const int id, const double dt, const vec &s):
	basis(id), nsteps((int)s.n_rows), dt(dt),
    series(vec(s))
{
}

timeseries::timeseries(const int id, const double dt, char* fileName):
	basis(id), dt(dt)
{
	series.load(fileName, raw_ascii);
	nsteps = (int)series.n_rows;
}

timeseries::timeseries(const int id, const double dt) :
	basis(id), dt(dt), nsteps(0)
{
}

timeseries::~timeseries()
{
}

void timeseries::load(char *fileName)
{
	series.load(fileName, raw_ascii);
	nsteps = (int)series.n_rows;
}
