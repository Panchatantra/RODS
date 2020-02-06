#include "spring2D.h"

spring2D::spring2D(const int id, node * nodeI, node * nodeJ, double k, ELE::localAxis axis) :
	element2D(id, nodeI, nodeJ), k(k), ue(0.0), f(0.0)
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


spring2D::~spring2D()
{
}

void spring2D::buildMatrix()
{
	K = T.t()*k*T;
}

void spring2D::getResponse()
{
	u = vec({ nodeI->dofX->dsp, nodeI->dofZ->dsp, nodeJ->dofX->dsp, nodeJ->dofZ->dsp });

	ue = 0.0;
	for (size_t i = 0; i < 4; i++)
	{
		ue += T(i)*u(i);
	}

	f = k * ue;

	force = &f;
	deformation = &ue;
}

void spring2D::assembleStiffnessMatrix(mat &K)
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
