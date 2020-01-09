#include "spring.h"


spring::spring(const int id, dof *i, dof *j, const double k)
{
	this->id = id;
	this->k = k;

	dofI = i;
	dofJ = j;

	f = 0.0;

	buildMatrix();
}

spring::~spring()
{
}

void spring::buildMatrix()
{
	K(0, 0) = k;
	K(0, 1) = -k;
	K(1, 0) = -k;
	K(1, 1) = k;
}

void spring::getResponse()
{
	double u = dofJ->dsp - dofI->dsp;
	f = k * u;
}
