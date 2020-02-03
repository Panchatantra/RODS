#include "inerter2D.h"

inerter2D::inerter2D(const int id, node * nodeI, node * nodeJ, double m, ELE::localAxis axis) :
	element2D(id, nodeI, nodeJ), m(m),
	ue(0.0), f(0.0)
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

inerter2D::~inerter2D()
{
}

void inerter2D::buildMatrix()
{
	M = T.t()*m*T;
}

void inerter2D::assembleMassMatrix(mat & M)
{
	int local[4] = { 0,1,2,3 };
	int global[4] = { nodeI->dofX->eqnId, nodeI->dofY->eqnId,
					 nodeJ->dofX->eqnId, nodeJ->dofY->eqnId };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			M(global[i], global[j]) += this->M(local[i], local[j]);
		}
	}
}

void inerter2D::getResponse(const bool update)
{
	double *u = new double[4] {nodeI->dofX->dsp, nodeI->dofY->dsp, nodeJ->dofX->dsp, nodeJ->dofY->dsp};
	double *a = new double[4] {nodeI->dofX->acc, nodeI->dofY->acc, nodeJ->dofX->acc, nodeJ->dofY->acc};

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
