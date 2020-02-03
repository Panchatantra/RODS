#include "beamElastic.h"

beamElastic::beamElastic(const int id, node * nodeI, node * nodeJ, const double EI):
	element2D(id, nodeI, nodeJ), EI(EI), ue(new double[2]{ 0.0, 0.0 }), f(new double[2]{ 0.0, 0.0 })
{
	double dx = nodeJ->x - nodeI->x;
	double dy = nodeJ->y - nodeI->y;

	L = sqrt(dx*dx + dy*dy);

	k = mat({ {4*EI/L, 2*EI/L},
			  {2*EI/L, 4*EI/L} });

	lyx = -dy / L;
	lyy = dx / L;

	T = mat( {{lyx/L, lyy/L, 1.0, -lyy, lyx, 0.0},
		      {lyx/L, lyy/L, 0.0, -lyy, lyx, 1.0} });

	buildMatrix();

	nv = 2;
}


beamElastic::~beamElastic()
{
}

void beamElastic::buildMatrix()
{
	K = T.t()*k*T;
}

void beamElastic::getResponse(const bool update)
{
	u = vec( { nodeI->dofX->dsp, nodeI->dofY->dsp, nodeI->dofRZ->dsp,
		       nodeJ->dofX->dsp, nodeJ->dofY->dsp, nodeJ->dofRZ->dsp } );

	for (size_t i = 0; i < 3; i++)
	{
		ue[i] = 0.0;
		f[i] = 0.0;
	}

	for (size_t i = 0; i < 6; i++)
	{
		ue[0] += T(0, i)*u(i);
		ue[1] += T(1, i)*u(i);
	}

	f[0] = k(0, 0)*ue[0] + k(0, 1)*ue[1];
	f[1] = k(1, 0)*ue[0] + k(1, 1)*ue[1];

	force = f;
	deformation = ue;
}

void beamElastic::assembleStiffnessMatrix(mat &K)
{
	int local[6] = {0,1,2,3,4,5};
	int global[6] = {nodeI->dofX->eqnId, nodeI->dofY->eqnId, nodeI->dofRZ->eqnId,
					 nodeJ->dofX->eqnId, nodeJ->dofY->eqnId, nodeJ->dofRZ->eqnId };

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			K(global[i], global[j]) += this->K(local[i], local[j]);
		}
	}
}
