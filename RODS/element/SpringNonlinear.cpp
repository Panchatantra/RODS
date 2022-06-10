#include "SpringNonlinear.h"


SpringNonlinear::SpringNonlinear(const int id, DOF *i, DOF *j, Material1D *smat) :
	ROD1D(id,i,j), smat(smat->copy()), k(smat->E), u(0.0), f(0.0)
{
	buildMatrix();
}


SpringNonlinear::~SpringNonlinear()
{
}

void SpringNonlinear::buildMatrix()
{
	K(0, 0) = k;
	K(0, 1) = -k;
	K(1, 0) = -k;
	K(1, 1) = k;
}

void SpringNonlinear::assembleStiffnessMatrix(mat & K)
{
	int i_local = 0;
	int j_local = 1;

	int i_global = dofI->eqnId;
	int j_global = dofJ->eqnId;

	K(i_global, i_global) += this->K(i_local, i_local);
	K(i_global, j_global) += this->K(i_local, j_local);
	K(j_global, i_global) += this->K(j_local, i_local);
	K(j_global, j_global) += this->K(j_local, j_local);
}

void SpringNonlinear::getResponse(const bool update)
{
	u = dofJ->dsp - dofI->dsp;
	smat->setStrain(&u);
	smat->getResponse(update);

	k = smat->E;
	f = smat->sigma;

	//q(0) = -(f - k*u);
	q(0) = -f;
	q(1) = f;

	force = &f;
	deformation = &u;

	buildMatrix();
}

void SpringNonlinear::assembleNonlinearForceVector(vec & q)
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