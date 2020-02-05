#include "dof.h"


dof::dof(const int n, direction dir) :
	mass(0.0), dir(dir), isFixed(false), dsp(0.0), vel(0.0), acc(0.0), eqnId(0), load(0.0)
{
	id = n;
}

dof::dof(const int n, direction dir, const double m, const bool fixed) :
	dir(dir), dsp(0.0), vel(0.0), acc(0.0), eqnId(0), load(0.0)
{
	id = n;
	mass = m;
	isFixed = fixed;
}

dof::~dof()
{
}
