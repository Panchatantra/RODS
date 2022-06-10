#include "Dashpot3D.h"

Dashpot3D::Dashpot3D(const int id, Node * nodeI, Node * nodeJ, double c, RODS::LocalAxis axis) :
	ROD3D(id, nodeI, nodeJ), c(c), ue(0.0), f(0.0)
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

Dashpot3D::~Dashpot3D()
{
}

void Dashpot3D::buildMatrix()
{
	C = T.t()*c*T;
}

void Dashpot3D::assembleDampingMatrix(mat & C)
{
	int local[6] = { 0,1,2,3,4,5 };
	int global[6] = { nodeI->dofX->eqnId, nodeI->dofY->eqnId, nodeI->dofZ->eqnId,
					  nodeJ->dofX->eqnId, nodeJ->dofY->eqnId, nodeJ->dofZ->eqnId };

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			C(global[i], global[j]) += this->C(local[i], local[j]);
		}
	}
}

void Dashpot3D::getResponse(const bool update)
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

	f = c * ve;

	force = &f;
	deformation = &ue;
}
