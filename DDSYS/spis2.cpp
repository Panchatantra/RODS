#include "spis2.h"

spis2::spis2(const int n, dof * i, dof * j, dof *in, const double m, const double c, const double k)
{
	id = n;
	this->m = m;
	this->c = c;
	this->k = k;

	dofI = i;
	dofJ = j;
	dofIN = in;

	f = 0.0;
	buildMatrix();
}


spis2::~spis2()
{
}

void spis2::buildMatrix()
{
	M = zeros<mat>(3, 3);
	C = zeros<mat>(3, 3);
	K = k*ones<mat>(3, 3);
	
	M(1, 1) = m;
	C(1, 1) = c;

	K(0, 2) *= -1.0;
	K(1, 2) *= -1.0;
	K(2, 0) *= -1.0;
	K(2, 1) *= -1.0;
}

void spis2::assembleMassMatrix(mat & M)
{
	int in_local = 1;
	int in_global = this->dofIN->eqnId;
	M(in_global, in_global) += this->M(in_local, in_local);
}

void spis2::assembleStiffnessMatrix(mat & K)
{
	int in_global = this->dofIN->eqnId;

	int i_local = 0;
	int in_local = 1;
	int j_local = 2;

	if (this->dofI->isFixed)
	{
		int j_global = this->dofJ->eqnId;
		K(in_global, in_global) += this->K(in_local, in_local);
		K(in_global, j_global) += this->K(in_local, j_local);
		K(j_global, in_global) += this->K(j_local, in_local);
		K(j_global, j_global) += this->K(j_local, j_local);
	}
	else if (this->dofJ->isFixed)
	{
		int i_global = this->dofI->eqnId;
		K(in_global, in_global) += this->K(in_local, in_local);
		K(in_global, i_global) += this->K(in_local, i_local);
		K(i_global, in_global) += this->K(i_local, in_local);
		K(i_global, i_global) += this->K(i_local, i_local);
	}
	else
	{
		int i_global = this->dofI->eqnId;
		int j_global = this->dofJ->eqnId;

		K(i_global, i_global) += this->K(i_local, i_local);
		K(i_global, j_global) += this->K(i_local, j_local);
		K(i_global, in_global) += this->K(i_local, in_local);
		K(j_global, i_global) += this->K(j_local, i_local);
		K(j_global, j_global) += this->K(j_local, j_local);
		K(j_global, in_global) += this->K(j_local, in_local);
		K(in_global, i_global) += this->K(in_local, i_local);
		K(in_global, j_global) += this->K(in_local, j_local);
		K(in_global, in_global) += this->K(in_local, in_local);
	}
}

void spis2::assembleDampingMatrix(mat & C)
{
	int in_local = 1;
	int in_global = this->dofIN->eqnId;
	C(in_global, in_global) += this->C(in_local, in_local);
}
