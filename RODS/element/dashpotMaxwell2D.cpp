#include "dashpotMaxwell2D.h"

dashpotMaxwell2D::dashpotMaxwell2D(const int id, Node * nodeI, Node * nodeJ, const double k, const double c, const double alpha,
	ELE::localAxis axis) :
	Element2D(id, nodeI, nodeJ), k(k), c(c), alpha(alpha),
	ue(0.0), f(0.0), ud(0.0), udp(0.0)
{
	if (axis == ELE::U1)
	{
		T = rowvec({ -lxx , -lxy , lxx , lxy });
	}
	else
	{
		T = rowvec({ -lyx , -lyy , lyx , lyy });
	}
}

dashpotMaxwell2D::~dashpotMaxwell2D()
{
}

void dashpotMaxwell2D::getResponse(const bool update)
{
	double *u = new double[4] {nodeI->dofX->dsp, nodeI->dofZ->dsp,
							   nodeJ->dofX->dsp, nodeJ->dofZ->dsp};
	double *v = new double[4] {nodeI->dofX->vel, nodeI->dofZ->vel,
							   nodeJ->dofX->vel, nodeJ->dofZ->vel};

	ue = 0.0;
	double ve = 0.0;
	for (size_t i = 0; i < 4; i++)
	{
		ue += T(i)*u[i];
		ve += T(i)*v[i];
	}

	ud = udp;

	f = k*(ue-ud);

	double f0, f1;

	f0 = sign(ue - ud)*pow(abs(f)/c, 1.0/alpha);
	ud = udp + Element::dt*f0;

	for (size_t i=0; i<1; i++)
	{
		f1 = sign(ue + dt*ve - ud)*pow(k*abs(ue + dt*ve - ud) / c, 1.0 / alpha);
		ud = udp + Element::dt*0.5*(f0 + f1);
	}

	for (size_t i = 0; i < 4; i++)
	{
		q(i) = T(i)*f;
	}

	force = &f;
	deformation = &ue;

	if (update)
	{
		udp = ud;
	}
}

void dashpotMaxwell2D::assembleNonlinearForceVector(vec &q)
{
	int local[4] = {0,1,2,3};
	int global[4] = {nodeI->dofX->eqnId, nodeI->dofZ->eqnId,
					 nodeJ->dofX->eqnId, nodeJ->dofZ->eqnId };
	bool fixed[4] = {nodeI->dofX->isFixed, nodeI->dofZ->isFixed,
					 nodeJ->dofX->isFixed, nodeJ->dofZ->isFixed };

	for (int i = 0; i < 4; i++)
	{
		if (!fixed[i])
		{
			q(global[i]) += this->q(local[i]);
		}
	}
}