#include "dof.h"


dof::dof(const int n, direction dir) :
    eqnId(0), mass(0.0), dir(dir), load(0.0), isFixed(false),
    dsp(0.0), vel(0.0), acc(0.0)
{
	id = n;
}

dof::dof(const int n, direction dir, const double m, const bool fixed) :
    eqnId(0), dir(dir), load(0.0),
    dsp(0.0), vel(0.0), acc(0.0)
{
	id = n;
	mass = m;
	isFixed = fixed;
}

dof::~dof()
{
}

void dof::setResponse(const double dsp, const double vel, const double acc)
{
	this->dsp = dsp;
	this->vel = vel;
	this->acc = acc;
}

void dof::setLoad(const double load)
{
	this->load = load;
}

void dof::addLoad(const double load)
{
	this->load += load;
}

double dof::g = 9800.0;
