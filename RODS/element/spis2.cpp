#include "spis2.h"

spis2::spis2(const int id, dof * i, dof * j, dof *in, const double m, const double c, const double k) :
	element1D(id, i, j), dofIN(in), m(m), c(c), k(k),
	u(new double[2] {0.0,0.0}), f(new double[2] {0.0,0.0})
{
	buildMatrix();
	nv = 2;
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
	int in_global = dofIN->eqnId;

	int i_local = 0;
	int in_local = 1;
	int j_local = 2;

	int i_global = dofI->eqnId;
	int j_global = dofJ->eqnId;

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

void spis2::assembleDampingMatrix(mat & C)
{
	int in_local = 1;
	int in_global = dofIN->eqnId;
	C(in_global, in_global) += this->C(in_local, in_local);
}

void spis2::getResponse(const bool update)
{
	u[0] = dofJ->dsp - dofI->dsp;
	u[1] = dofIN->dsp;
	f[0] = k * (u[0] - u[1]);
	f[1] = m*dofIN->acc;

	force = f;
	deformation = u;
}
