#include "Dashpot2D.h"

Dashpot2D::Dashpot2D(const int id, Node * nodeI, Node * nodeJ, double c, ELE::LocalAxis axis) :
	Element2D(id, nodeI, nodeJ), c(c), ue(0.0), f(0.0)
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

Dashpot2D::~Dashpot2D()
{
}

void Dashpot2D::buildMatrix()
{
	C = T.t()*c*T;
}

void Dashpot2D::assembleDampingMatrix(mat & C)
{
	int local[4] = { 0,1,2,3 };
	int global[4] = { nodeI->dofX->eqnId, nodeI->dofZ->eqnId,
					  nodeJ->dofX->eqnId, nodeJ->dofZ->eqnId };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			C(global[i], global[j]) += this->C(local[i], local[j]);
		}
	}
}

void Dashpot2D::getResponse(const bool update)
{
	double *u = new double[4] {nodeI->dofX->dsp, nodeI->dofZ->dsp,
							   nodeJ->dofX->dsp, nodeJ->dofZ->dsp};
	double *v = new double[4] {nodeI->dofX->vel, nodeI->dofZ->vel,
							   nodeJ->dofX->vel, nodeJ->dofZ->vel};

	ue = 0.0;
	double ve = 0.0;
	for (size_t i = 0; i < 4; i++)
	{
		ue += T(i)*u[i];
		ve += T(i)*v[i];
	}

	f = c * ve;

	force = &f;
	deformation = &ue;
}
