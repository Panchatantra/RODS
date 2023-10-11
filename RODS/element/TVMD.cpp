#include "TVMD.h"

TVMD::TVMD() :
	ROD1D(), m(0.0), c(0.0), k(0.0),
	u(0.0), f(0.0),
	ud(0.0), vd(0.0), udp(0.0), vdp(0.0)
{
}

TVMD::TVMD(const int id, DOF *i, DOF *j,
			const double m, const double c, const double k) :
	ROD1D(id, i, j), m(m), c(c), k(k),
	u(0.0), f(0.0),
	ud(0.0), vd(0.0), udp(0.0), vdp(0.0)
{
}


TVMD::~TVMD()
{
}

void TVMD::getResponse(const bool update)
{
	u = dofJ->dsp - dofI->dsp;
	double v = dofJ->vel - dofI->vel;

	ud = udp;
	vd = vdp;
	f = k*(u - ud);

	double dt = Element::dt;

	//Modified Euler Method:
	ud = ud + dt*vd;
	vd = vd + dt*(-c*vd+f)/m;

	ud = udp + dt*0.5*(vdp + vd);
	vd = vdp + dt*0.5*((-c*vdp + f) + (-c*vd + k*(u + dt*v - ud)))/m;

	//Runge-Kutta Method:

	//double kud1, kvd1, kud2, kvd2, kud3, kvd3, kud4, kvd4;
	//func(u, ud, vd, kud1, kvd1);
	//func(u + v * dt*0.5, ud + 0.5*kud1, vd + 0.5*kvd1, kud2, kvd2);
	//func(u + v * dt*0.5, ud + 0.5*kud2, vd + 0.5*kvd2, kud3, kvd3);
	//func(u + v * dt, ud + kud3, vd + kvd3, kud4, kvd4);

	//ud = udp + 1.0 / 6.0*(kud1 + 2.0*kud2 + 2.0*kud3 + kud4);
	//vd = vdp + 1.0 / 6.0*(kvd1 + 2.0*kvd2 + 2.0*kvd3 + kvd4);

	q(0) = -f;
	q(1) = f;

	force = &f;
	deformation = &u;

	if (update)
	{
		udp = ud;
		vdp = vd;
	}
}

void TVMD::assembleNonlinearForceVector(vec & q)
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

void TVMD::func(const double uc, const double ud, const double vd, double &kud, double &kvd)
{
	kud = Element::dt*vd;
	kvd = Element::dt*(-c*vd+k*(uc-ud))/m;
}
