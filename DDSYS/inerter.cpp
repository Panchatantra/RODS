#include "inerter.h"



inerter::inerter(const int n, dof *i, dof *j)
{
	num = n;
	inertance = 1.0e-9;

	dofI = i;
	dofJ = j;

	force = 0.0;
	buildMatrix();
}


inerter::inerter(const int n, dof *i, dof *j, const double m)
{
	num = n;
	inertance = m;

	dofI = i;
	dofJ = j;

	force = 0.0;

	buildMatrix();
}

inerter::~inerter()
{
}

void inerter::buildMatrix()
{
	M(0, 0) = inertance;
	M(0, 1) = -inertance;
	M(1, 0) = -inertance;
	M(1, 1) = inertance;
}
