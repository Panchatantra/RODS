#include "dof.h"



dof::dof(const int n, node *loc, direction dir)
{
	id = n;
	this->loc = loc;
	mass = 1.0e-9;

	isFixed = false;

	dsp = 0.0;
	vel = 0.0;
	acc = 0.0;
}

dof::dof(const int n, node *loc, direction dir, const double m, const bool fixed)
{
	id = n;
	this->loc = loc;
	mass = m;

	isFixed = fixed;

	dsp = 0.0;
	vel = 0.0;
	acc = 0.0;
}

dof::~dof()
{
}
