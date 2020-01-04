#include "spring.h"


spring::spring(const int n, dof *i, dof *j, const double k)
{
	id = n;
	stiff = k;

	dofI = i;
	dofJ = j;

	force = 0.0;

	buildMatrix();
}

spring::~spring()
{
}

void spring::buildMatrix()
{
	K(0, 0) = stiff;
	K(0, 1) = -stiff;
	K(1, 0) = -stiff;
	K(1, 1) = stiff;
}
