#include "DashpotMaxwell.h"


DashpotMaxwell::DashpotMaxwell(const int id, DOF *i, DOF *j, const double ks, const double c, const double alpha) :
	Element1D(id, i, j), ks(ks), c(c), alpha(alpha),
	u(0.0), f(0.0), ud(0.0), udp(0.0)
{
}


DashpotMaxwell::~DashpotMaxwell()
{
}

void DashpotMaxwell::getResponse(const bool update)
{
	u = dofJ->dsp - dofI->dsp;
	double v = dofJ->vel - dofI->vel;

	ud = udp;

	f = ks*(u-ud);

	double f0 = sign(u-ud)*pow(f/c, 1.0/alpha);
	ud = udp + Element::dt*f0;
	double f1 = sign(u+dt*v-ud)*pow(ks*(u+dt*v-ud)/c, 1.0/alpha);
	ud = udp + Element::dt*0.5*(f0 + f1);

	q(0) = -f;
	q(1) = f;

	force = &f;
	deformation = &u;

	if (update)
	{
		udp = ud;
	}
}

void DashpotMaxwell::assembleNonlinearForceVector(vec & q)
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
