#include "SpringNonlinear2D.h"

SpringNonlinear2D::SpringNonlinear2D(const int id, Node * nodeI, Node * nodeJ, Material1D *smat, ELE::localAxis axis) :
	Element2D(id, nodeI, nodeJ), smat(smat->copy()), k(smat->E), ue(0.0), f(0.0), axis(axis)
{
	if (axis == ELE::U1)
	{
		T = rowvec({ -lxx , -lxy , lxx , lxy });
	}
	else
	{
		T = rowvec({ -lyx , -lyy , lyx , lyy });
	}
	buildMatrix();
}

SpringNonlinear2D::~SpringNonlinear2D()
{
}

void SpringNonlinear2D::buildMatrix()
{
	K = T.t()*k*T;
}

void SpringNonlinear2D::getResponse(const bool update)
{
	double *u = new double[4] {nodeI->dofX->dsp, nodeI->dofZ->dsp,
							   nodeJ->dofX->dsp, nodeJ->dofZ->dsp};

	ue = 0.0;
	for (size_t i = 0; i < 4; i++)
	{
		ue += T(i)*u[i];
	}

	smat->setStrain(&ue);
	smat->getResponse(update);

	k = smat->E;
	f = smat->sigma;

	for (size_t i = 0; i < 4; i++)
	{
		q(i) = T(i)*f;
	}

	force = &f;
	deformation = &ue;

	buildMatrix();
}

void SpringNonlinear2D::assembleNonlinearForceVector(vec &q)
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

void SpringNonlinear2D::assembleStiffnessMatrix(mat &K)
{
	int local[4] = { 0,1,2,3 };
	int global[4] = { nodeI->dofX->eqnId, nodeI->dofZ->eqnId,
					  nodeJ->dofX->eqnId, nodeJ->dofZ->eqnId };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			K(global[i], global[j]) += this->K(local[i], local[j]);
		}
	}
}