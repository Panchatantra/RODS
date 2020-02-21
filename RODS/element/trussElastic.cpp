#include "trussElastic.h"

trussElastic::trussElastic(const int id, Node * nodeI, Node * nodeJ, const double EA):
	Element2D(id, nodeI, nodeJ), EA(EA), ue(0.0), f(0.0)
{
	T = rowvec( {-lxx, -lxy, lxx, lxy} );
	k = EA/L;
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
	u = vec( { nodeI->dofX->dsp, nodeI->dofZ->dsp, nodeJ->dofX->dsp, nodeJ->dofZ->dsp } );

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
	int global[4] = {nodeI->dofX->eqnId, nodeI->dofZ->eqnId,
					 nodeJ->dofX->eqnId, nodeJ->dofZ->eqnId};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			K(global[i], global[j]) += this->K(local[i], local[j]);
		}
	}
}
