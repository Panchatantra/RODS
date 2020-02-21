#include "spring.h"

spring::spring(const int id, DOF *i, DOF *j, const double k) :
	Element1D(id, i, j), k(k), u(0.0), f(0.0)
{
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

	int i_global = this->dofI->eqnId;
	int j_global = this->dofJ->eqnId;

	K(i_global, i_global) += this->K(i_local, i_local);
	K(i_global, j_global) += this->K(i_local, j_local);
	K(j_global, i_global) += this->K(j_local, i_local);
	K(j_global, j_global) += this->K(j_local, j_local);
}

void spring::getResponse(const bool update)
{
	u = dofJ->dsp - dofI->dsp;
	f = k * u;

	force = &f;
	deformation = &u;
}