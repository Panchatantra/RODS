#include "dashpot.h"

dashpot::dashpot(const int n, dof * i, dof * j, const double c)
{
    id = n;
    this->c = c;

    dofI = i;
    dofJ = j;

    f = 0.0;

    buildMatrix();
}


dashpot::~dashpot()
{
}

void dashpot::buildMatrix()
{
	C(0, 0) = c;
	C(0, 1) = -c;
	C(1, 0) = -c;
	C(1, 1) = c;
}
