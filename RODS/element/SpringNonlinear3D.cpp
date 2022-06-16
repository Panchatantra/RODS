#include "SpringNonlinear3D.h"

SpringNonlinear3D::SpringNonlinear3D(const int id, Node * nodeI, Node * nodeJ, Material1D *smat, RODS::LocalAxis axis) :
	ROD3D(id, nodeI, nodeJ), smat(smat->copy()),
	k(smat->E), ue(0.0), f(0.0),
	axis(axis)
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

SpringNonlinear3D::~SpringNonlinear3D()
{
}

void SpringNonlinear3D::buildMatrix()
{
	K = T.t()*k*T;
}

void SpringNonlinear3D::getResponse(const bool update)
{
	double *u = new double[6] {nodeI->dofX->dsp, nodeI->dofY->dsp, nodeI->dofZ->dsp,
							   nodeJ->dofX->dsp, nodeJ->dofY->dsp, nodeJ->dofZ->dsp};

	ue = 0.0;
	for (size_t i = 0; i < 6; i++)
	{
		ue += T(i)*u[i];
	}

	smat->setStrain(&ue);
	smat->getResponse(update);

	k = smat->E;
	f = smat->sigma;

	for (size_t i = 0; i < 6; i++)
	{
		q(i) = T(i)*f;
	}

	force = &f;
	deformation = &ue;

	buildMatrix();
}

void SpringNonlinear3D::assembleNonlinearForceVector(vec &q)
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

void SpringNonlinear3D::assembleStiffnessMatrix(mat &K)
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