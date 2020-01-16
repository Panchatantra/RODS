#include "dashpot.h"

dashpot::dashpot(const int n, dof * i, dof * j, const double c) :
	u(0.0), f(0.0)
{
    id = n;
    this->c = c;

    dofI = i;
    dofJ = j;

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

void dashpot::assembleDampingMatrix(mat & C)
{
	int i_local = 0;
	int j_local = 1;

	if (this->dofI->isFixed)
	{
		int j_global = this->dofJ->eqnId;
		C(j_global, j_global) += this->C(j_local, j_local);
	}
	else if (this->dofJ->isFixed)
	{
		int i_global = this->dofI->eqnId;
		C(i_global, i_global) += this->C(i_local, i_local);
	}
	else
	{
		int i_global = this->dofI->eqnId;
		int j_global = this->dofJ->eqnId;

		C(i_global, i_global) += this->C(i_local, i_local);
		C(i_global, j_global) += this->C(i_local, j_local);
		C(j_global, i_global) += this->C(j_local, i_local);
		C(j_global, j_global) += this->C(j_local, j_local);
	}
}

void dashpot::getResponse(const bool update)
{
	u = dofJ->dsp - dofI->dsp;
	double v = dofJ->vel - dofI->vel;
	f = c*v;

	force = &f;
	deformation = &u;
}
