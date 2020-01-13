#include "dashpotExp.h"

dashpotExp::dashpotExp(const int n, dof * i, dof * j, const double c, const double alpha)
{
    id = n;
    this->c = c;

    dofI = i;
    dofJ = j;

    f = 0.0;

    buildMatrix();
}


dashpotExp::~dashpotExp()
{
}

void dashpotExp::buildMatrix()
{
	C(0, 0) = 0.0;
	C(0, 1) = -0.0;
	C(1, 0) = -0.0;
	C(1, 1) = 0.0;
}

void dashpotExp::assembleDampingMatrix(mat & C)
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

void dashpotExp::getResponse(const bool update)
{
	v = dofJ->vel - dofI->vel;
	f = c * pow(abs(v), alpha);

	q(0) = -f;
	q(1) = f;
}

void dashpotExp::assembleNonlinearForceVector(vec & q)
{
	int i_local = 0;
	int j_local = 1;

	if (this->dofI->isFixed)
	{
		int j_global = this->dofJ->eqnId;
		q(j_global) += this->q(j_local);
	}
	else if (this->dofJ->isFixed)
	{
		int i_global = this->dofI->eqnId;
		q(i_global) += this->q(i_local);
	}
	else
	{
		int i_global = this->dofI->eqnId;
		int j_global = this->dofJ->eqnId;

		q(i_global) += this->q(i_local);
		q(j_global) += this->q(j_local);
	}
}