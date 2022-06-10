#include "TVMD3D.h"

TVMD3D::TVMD3D(const int id, Node * nodeI, Node * nodeJ, const double m, const double c, const double k, RODS::LocalAxis axis) :
	ROD3D(id, nodeI, nodeJ), m(m), c(c), k(k),
	ue(0.0), f(0.0),
	ud(0.0), vd(0.0), udp(0.0), vdp(0.0)
{
	if (axis == RODS::LocalAxis::U1)
	{
		T = rowvec({ -lxx, -lxy, -lxz, lxx, lxy, lxz });
	}
	else if (axis == RODS::LocalAxis::U2)
	{
		T = rowvec({ -lyx, -lyy, -lyz, lyx, lyy, lyz });
	}
	else
	{
		T = rowvec({ -lzx, -lzy, -lzz, lzx, lzy, lzz });
	}
	buildMatrix();
}

TVMD3D::~TVMD3D()
{
}

void TVMD3D::getResponse(const bool update)
{
	double *u = new double[6] {nodeI->dofX->dsp, nodeI->dofY->dsp, nodeI->dofZ->dsp,
							   nodeJ->dofX->dsp, nodeJ->dofY->dsp, nodeJ->dofZ->dsp};
	double *v = new double[6] {nodeI->dofX->vel, nodeI->dofY->vel, nodeI->dofZ->vel,
							   nodeJ->dofX->vel, nodeJ->dofY->vel, nodeJ->dofZ->vel};

	ue = 0.0;
	double ve = 0.0;
	for (size_t i = 0; i < 6; i++)
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

	for (auto i = 0; i < 6; i++)
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

void TVMD3D::assembleNonlinearForceVector(vec &q)
{
	int local[6] = { 0,1,2,3,4,5 };
	int global[6] = { nodeI->dofX->eqnId, nodeI->dofY->eqnId, nodeI->dofZ->eqnId,
					  nodeJ->dofX->eqnId, nodeJ->dofY->eqnId, nodeJ->dofZ->eqnId };
	bool fixed[6] = {nodeI->dofX->isFixed, nodeI->dofY->isFixed, nodeI->dofZ->isFixed,
					 nodeJ->dofX->isFixed, nodeJ->dofY->isFixed, nodeJ->dofZ->isFixed };

	for (int i = 0; i < 6; i++)
	{
		if (!fixed[i])
		{
			q(global[i]) += this->q(local[i]);
		}
	}
}