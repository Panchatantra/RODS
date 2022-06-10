#include "Slider.h"


Slider::Slider(const int id, DOF *i, DOF *j, const double muN) :
	ROD1D(id, i, j), muN(muN),
	u(0.0), f(0.0), v(0.0)
{
}


Slider::~Slider()
{
}

void Slider::getResponse(const bool update)
{
	u = dofJ->dsp - dofI->dsp;
	v = dofJ->vel - dofI->vel;

	f = sign(v)*muN;

	//q(0) = -(f - k*u);
	q(0) = -f;
	q(1) = f;

	force = &f;
	deformation = &u;
}

void Slider::assembleNonlinearForceVector(vec & q)
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