#include "Spring3D.h"

Spring3D::Spring3D(const int id, Node * nodeI, Node * nodeJ, double k, RODS::LocalAxis axis) :
	ROD3D(id, nodeI, nodeJ), k(k), ue(0.0), f(0.0)
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


Spring3D::~Spring3D()
{
}

void Spring3D::buildMatrix()
{
	K = T.t()*k*T;
}

void Spring3D::getResponse()
{
	double *u = new double[6] {nodeI->dofX->dsp, nodeI->dofY->dsp, nodeI->dofZ->dsp,
							   nodeJ->dofX->dsp, nodeJ->dofY->dsp, nodeJ->dofZ->dsp};

	ue = 0.0;
	for (size_t i = 0; i < 6; i++)
	{
		ue += T(i)*u[i];
	}

	f = k * ue;

	force = &f;
	deformation = &ue;
}

void Spring3D::assembleStiffnessMatrix(mat &K)
{
	int local[6] = { 0,1,2,3,4,5 };
	int global[6] = { nodeI->dofX->eqnId, nodeI->dofY->eqnId, nodeI->dofZ->eqnId,
					  nodeJ->dofX->eqnId, nodeJ->dofY->eqnId, nodeJ->dofZ->eqnId };

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			K(global[i], global[j]) += this->K(local[i], local[j]);
		}
	}
}
