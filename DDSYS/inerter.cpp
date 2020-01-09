#include "inerter.h"


inerter::inerter(const int n, dof *i, dof *j, const double m)
{
	id = n;
	this->m = m;

	dofI = i;
	dofJ = j;

	f = 0.0;

	buildMatrix();
}

inerter::~inerter()
{
}

void inerter::buildMatrix()
{
	M(0, 0) = m;
	M(0, 1) = -m;
	M(1, 0) = -m;
	M(1, 1) = m;
}
