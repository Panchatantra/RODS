#include "Wave.h"

Wave::Wave(const int id, const double dt, const vec &s):
	Basis(id), nsteps((int)s.n_rows), dt(dt), series(vec(s))
{
}

Wave::Wave(const int id, const double dt, char* fileName):
	Basis(id), dt(dt)
{
	series.load(fileName, raw_ascii);
	nsteps = (int)series.n_rows;
}

Wave::Wave(const int id, const double dt) :
	Basis(id), dt(dt), nsteps(0)
{
}

Wave::~Wave()
{
}

void Wave::load(char *fileName)
{
	series.load(fileName, raw_ascii);
	nsteps = (int)series.n_rows;
}
