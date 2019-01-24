#include "dof.h"



dof::dof(const int n)
{
	num = n;
	mass = 1.0e-9;

	isFixed = false;
	isRelative = false;

	dsp = 0.0;
	vel = 0.0;
	acc = 0.0;
}

dof::dof(const int n, const double m, const bool fixed)
{
	num = n;
	mass = m;

	isFixed = fixed;
	isRelative = false;

	dsp = 0.0;
	vel = 0.0;
	acc = 0.0;
}

dof::dof(const int n, const bool rel)
{
	num = n;
	mass = 0.0;

	isFixed = false;
	isRelative = rel;

	dsp = 0.0;
	vel = 0.0;
	acc = 0.0;
}

dof::~dof()
{
}
