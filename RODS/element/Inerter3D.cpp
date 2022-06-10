#include "Inerter3D.h"

Inerter3D::Inerter3D(const int id, Node * nodeI, Node * nodeJ, double m, RODS::LocalAxis axis) :
	ROD3D(id, nodeI, nodeJ), m(m),
	ue(0.0), f(0.0)
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

Inerter3D::~Inerter3D()
{
}

void Inerter3D::buildMatrix()
{
	M = T.t()*m*T;
}

void Inerter3D::assembleMassMatrix(mat &M)
{
	int local[6] = { 0,1,2,3,4,5 };
	int global[6] = { nodeI->dofX->eqnId, nodeI->dofY->eqnId, nodeI->dofZ->eqnId,
					  nodeJ->dofX->eqnId, nodeJ->dofY->eqnId, nodeJ->dofZ->eqnId };

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			M(global[i], global[j]) += this->M(local[i], local[j]);
		}
	}
}

void Inerter3D::getResponse(const bool update)
{
	double *u = new double[6] {nodeI->dofX->dsp, nodeI->dofY->dsp, nodeI->dofZ->dsp,
							   nodeJ->dofX->dsp, nodeJ->dofY->dsp, nodeJ->dofZ->dsp};
	double *a = new double[6] {nodeI->dofX->acc, nodeI->dofY->acc, nodeI->dofZ->acc,
							   nodeJ->dofX->acc, nodeJ->dofY->acc, nodeJ->dofZ->acc};

	ue = 0.0;
	double ae = 0.0;
	for (size_t i = 0; i < 6; i++)
	{
		ue += T(i)*u[i];
		ae += T(i)*a[i];
	}

	f = m*ae;

	force = &f;
	deformation = &ue;
}
