#include "TVMD2D.h"

TVMD2D::TVMD2D(const int id, Node * nodeI, Node * nodeJ, const double m, const double c, const double k, RODS::LocalAxis axis) :
	ROD2D(id, nodeI, nodeJ), m(m), c(c), k(k),
	ue(0.0), f(0.0),
	ud(0.0), vd(0.0), udp(0.0), vdp(0.0)
{
	if (axis == RODS::LocalAxis::U1)
	{
		T = rowvec({ -lxx , -lxz , lxx , lxz });
	}
	else
	{
		T = rowvec({ -lzx , -lzz , lzx , lzz });
	}
	buildMatrix();
}

TVMD2D::~TVMD2D()
{
}

void TVMD2D::getResponse(const bool update)
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
	vd = vdp;

	f = k*(ue - ud);

	double dt = Element::dt;

	//Modified Euler Method:
	ud = ud + dt*vd;
	vd = vd + dt*(-c*vd+f)/m;

	ud = udp + dt*0.5*(vdp + vd);
	vd = vdp + dt*0.5*((-c*vdp + f) + (-c*vd + k*(ue + dt*ve - ud)))/m;

	for (auto i = 0; i < 4; i++)
	{
		q(i) = T(i)*f;
	}

	force = &f;
	deformation = &ue;

	if (update)
	{
		udp = ud;
		vdp = vd;
	}
}

void TVMD2D::assembleNonlinearForceVector(vec &q)
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