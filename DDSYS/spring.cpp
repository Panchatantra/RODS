#include "spring.h"


spring::spring(const int id, dof *i, dof *j, const double k) :
	u(0.0), f(0.0)
{
	this->id = id;
	this->k = k;

	dofI = i;
	dofJ = j;

	buildMatrix();
}

spring::~spring()
{
}

void spring::buildMatrix()
{
	K(0, 0) = k;
	K(0, 1) = -k;
	K(1, 0) = -k;
	K(1, 1) = k;
}

void spring::assembleStiffnessMatrix(mat & K)
{
	int i_local = 0;
	int j_local = 1;

	if (this->dofI->isFixed)
	{
		int j_global = this->dofJ->eqnId;
		K(j_global, j_global) += this->K(j_local, j_local);
	}
	else if (this->dofJ->isFixed)
	{
		int i_global = this->dofI->eqnId;
		K(i_global, i_global) += this->K(i_local, i_local);
	}
	else
	{
		int i_global = this->dofI->eqnId;
		int j_global = this->dofJ->eqnId;

		K(i_global, i_global) += this->K(i_local, i_local);
		K(i_global, j_global) += this->K(i_local, j_local);
		K(j_global, i_global) += this->K(j_local, i_local);
		K(j_global, j_global) += this->K(j_local, j_local);
	}
}

void spring::getResponse()
{
	u = dofJ->dsp - dofI->dsp;
	f = k * u;
}
