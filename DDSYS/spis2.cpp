#include "spis2.h"

spis2::spis2(const int n, dof * i, dof * j, dof *in, const double m, const double c, const double k)
{
	id = n;
	inertance = m;
	damping = c;
	stiff = k;

	dofI = i;
	dofJ = j;
	dofIN = in;

	force = 0.0;
	buildMatrix();
}


spis2::~spis2()
{
}

void spis2::buildMatrix()
{
	M = zeros<mat>(3, 3);
	C = zeros<mat>(3, 3);
	K = stiff*ones<mat>(3, 3);
	
	M(1, 1) = inertance;
	C(1, 1) = damping;

	K(0, 2) *= -1.0;
	K(1, 2) *= -1.0;
	K(2, 0) *= -1.0;
	K(2, 1) *= -1.0;
}
