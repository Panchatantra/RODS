#include "spring2D.h"

spring2D::spring2D(const int id, node * nodeI, node * nodeJ, double k, ELE::localAxis axis) :
	element2D(id, nodeI, nodeJ), k(k)
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
	u = vec({ nodeI->dofX->dsp, nodeI->dofY->dsp, nodeJ->dofX->dsp, nodeJ->dofY->dsp });

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
	int global[4] = { nodeI->dofX->eqnId, nodeI->dofY->eqnId,
					 nodeJ->dofX->eqnId, nodeJ->dofY->eqnId };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			K(global[i], global[j]) += this->K(local[i], local[j]);
		}
	}
}
