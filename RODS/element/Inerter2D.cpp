#include "Inerter2D.h"

Inerter2D::Inerter2D(const int id, Node * nodeI, Node * nodeJ, double m, RODS::LocalAxis axis) :
	Element2D(id, nodeI, nodeJ), m(m),
	ue(0.0), f(0.0)
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

Inerter2D::~Inerter2D()
{
}

void Inerter2D::buildMatrix()
{
	M = T.t()*m*T;
}

void Inerter2D::assembleMassMatrix(mat &M)
{
	int local[4] = { 0,1,2,3 };
	int global[4] = { nodeI->dofX->eqnId, nodeI->dofZ->eqnId,
					  nodeJ->dofX->eqnId, nodeJ->dofZ->eqnId };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			M(global[i], global[j]) += this->M(local[i], local[j]);
		}
	}
}

void Inerter2D::getResponse(const bool update)
{
	double *u = new double[4] {nodeI->dofX->dsp, nodeI->dofZ->dsp,
							   nodeJ->dofX->dsp, nodeJ->dofZ->dsp};
	double *a = new double[4] {nodeI->dofX->acc, nodeI->dofZ->acc,
							   nodeJ->dofX->acc, nodeJ->dofZ->acc};

	ue = 0.0;
	double ae = 0.0;
	for (size_t i = 0; i < 4; i++)
	{
		ue += T(i)*u[i];
		ae += T(i)*a[i];
	}

	f = m * ae;

	force = &f;
	deformation = &ue;
}
