#include "inerter.h"


inerter::inerter(const int n, dof *i, dof *j, const double m) :
	u(0.0), f(0.0)
{
	id = n;
	this->m = m;

	dofI = i;
	dofJ = j;

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

void inerter::assembleMassMatrix(mat & M)
{
	int i_local = 0;
	int j_local = 1;

	if (this->dofI->isFixed)
	{
		int j_global = this->dofJ->eqnId;
		M(j_global, j_global) += this->M(j_local, j_local);
	}
	else if (this->dofJ->isFixed)
	{
		int i_global = this->dofI->eqnId;
		M(i_global, i_global) += this->M(i_local, i_local);
	}
	else
	{
		int i_global = this->dofI->eqnId;
		int j_global = this->dofJ->eqnId;

		M(i_global, i_global) += this->M(i_local, i_local);
		M(i_global, j_global) += this->M(i_local, j_local);
		M(j_global, i_global) += this->M(j_local, i_local);
		M(j_global, j_global) += this->M(j_local, j_local);
	}
}

void inerter::getResponse()
{
	u = dofJ->dsp - dofI->dsp;
	double a = dofJ->acc - dofI->acc;
	f = m * a;
}
