#include "DashpotExp.h"

DashpotExp::DashpotExp(const int id, DOF * i, DOF * j, const double c, const double alpha) :
	ROD1D(id, i, j), c(c), alpha(alpha),
	u(0.0), f(0.0)
{
    buildMatrix();
}

DashpotExp::~DashpotExp()
{
}

void DashpotExp::buildMatrix()
{
	C(0, 0) = 0.0;
	C(0, 1) = -0.0;
	C(1, 0) = -0.0;
	C(1, 1) = 0.0;
}

void DashpotExp::assembleDampingMatrix(mat & C)
{
	int i_local = 0;
	int j_local = 1;

	int i_global = dofI->eqnId;
	int j_global = dofJ->eqnId;

	C(i_global, i_global) += this->C(i_local, i_local);
	C(i_global, j_global) += this->C(i_local, j_local);
	C(j_global, i_global) += this->C(j_local, i_local);
	C(j_global, j_global) += this->C(j_local, j_local);
}

void DashpotExp::getResponse(const bool update)
{
	u = dofJ->dsp - dofI->dsp;
	double v = dofJ->vel - dofI->vel;
	f = c * sign(v)*pow(abs(v), alpha);

	q(0) = -f;
	q(1) = f;

	force = &f;
	deformation = &u;
}

void DashpotExp::assembleNonlinearForceVector(vec & q)
{
	int i_local = 0;
	int j_local = 1;

	if (dofI->isFixed)
	{
		int j_global = dofJ->eqnId;
		q(j_global) += this->q(j_local);
	}
	else if (dofJ->isFixed)
	{
		int i_global = dofI->eqnId;
		q(i_global) += this->q(i_local);
	}
	else
	{
		int i_global = dofI->eqnId;
		int j_global = dofJ->eqnId;

		q(i_global) += this->q(i_local);
		q(j_global) += this->q(j_local);
	}
}
