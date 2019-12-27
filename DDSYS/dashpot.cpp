#include "dashpot.h"



dashpot::dashpot(const int n, dof *i, dof *j)
{
    id = n;
    damping = 1.0e-9;

    dofI = i;
    dofJ = j;

    force = 0.0;
    buildMatrix();
}

dashpot::dashpot(const int n, dof * i, dof * j, const double c)
{
    id = n;
    damping = c;

    dofI = i;
    dofJ = j;

    force = 0.0;

    buildMatrix();
}


dashpot::~dashpot()
{
}

void dashpot::buildMatrix()
{
	C(0, 0) = damping;
	C(0, 1) = -damping;
	C(1, 0) = -damping;
	C(1, 1) = damping;
}
