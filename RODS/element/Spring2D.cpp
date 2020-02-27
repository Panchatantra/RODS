#include "Spring2D.h"

Spring2D::Spring2D(const int id, Node * nodeI, Node * nodeJ, double k, RODS::LocalAxis axis) :
	Element2D(id, nodeI, nodeJ), k(k), ue(0.0), f(0.0)
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


Spring2D::~Spring2D()
{
}

void Spring2D::buildMatrix()
{
	K = T.t()*k*T;
}

void Spring2D::getResponse()
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

void Spring2D::assembleStiffnessMatrix(mat &K)
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
