#include "trussElastic.h"

trussElastic::trussElastic(const int id, node * nodeI, node * nodeJ, const double EA):
	element2D(id, nodeI, nodeJ), EA(EA), ue(0.0), f(0.0)
{
	double dx = nodeJ->x - nodeI->x;
	double dy = nodeJ->y - nodeI->y;

	L = sqrt(dx*dx + dy*dy);

	k = EA / L;

	lxx = dx / L;
	lxy = dy / L;

	T = rowvec( {-lxx, -lxy, lxx, lxy} );

	buildMatrix();
}


trussElastic::~trussElastic()
{
}

void trussElastic::buildMatrix()
{
	K = T.t()*k*T;
}

void trussElastic::getResponse(const bool update)
{
	u = vec( { nodeI->dofX->dsp, nodeI->dofY->dsp, nodeJ->dofX->dsp, nodeJ->dofY->dsp } );

	ue = 0.0;
	for (size_t i = 0; i < 4; i++)
	{
		ue += T(i)*u(i);
	}

	f = k*ue;

	force = &f;
	deformation = &ue;
}

void trussElastic::assembleStiffnessMatrix(mat &K)
{
	int local[4] = {0,1,2,3};
	int global[4] = {nodeI->dofX->eqnId, nodeI->dofY->eqnId,
					 nodeJ->dofX->eqnId, nodeJ->dofY->eqnId};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			K(global[i], global[j]) += this->K(local[i], local[j]);
		}
	}
}
