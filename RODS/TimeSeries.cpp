#include "TimeSeries.h"

TimeSeries::TimeSeries(const int id, const double dt, const vec &s):
	Basis(id), nsteps((int)s.n_rows), dt(dt),
    series(vec(s))
{
}

TimeSeries::TimeSeries(const int id, const double dt, char* fileName):
	Basis(id), dt(dt)
{
	series.load(fileName, raw_ascii);
	nsteps = (int)series.n_rows;
}

TimeSeries::TimeSeries(const int id, const double dt) :
	Basis(id), dt(dt), nsteps(0)
{
}

TimeSeries::~TimeSeries()
{
}

void TimeSeries::load(char *fileName)
{
	series.load(fileName, raw_ascii);
	nsteps = (int)series.n_rows;
}
