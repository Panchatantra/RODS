#include "Inerter.h"


Inerter::Inerter(const int id, DOF *i, DOF *j, const double m) :
	ROD1D(id, i, j), m(m),
	u(0.0), f(0.0)
{
	buildMatrix();
}

Inerter::~Inerter()
{
}

void Inerter::buildMatrix()
{
	M(0, 0) = m;
	M(0, 1) = -m;
	M(1, 0) = -m;
	M(1, 1) = m;
}

void Inerter::assembleMassMatrix(mat & M)
{
	int i_local = 0;
	int j_local = 1;
	
	int i_global = this->dofI->eqnId;
	int j_global = this->dofJ->eqnId;

	M(i_global, i_global) += this->M(i_local, i_local);
	M(i_global, j_global) += this->M(i_local, j_local);
	M(j_global, i_global) += this->M(j_local, i_local);
	M(j_global, j_global) += this->M(j_local, j_local);
}

void Inerter::getResponse(const bool update)
{
	u = dofJ->dsp - dofI->dsp;
	double a = dofJ->acc - dofI->acc;
	f = m * a;

	force = &f;
	deformation = &u;
}
