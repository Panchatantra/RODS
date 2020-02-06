#include "truss.h"

truss::truss(const int id, node * nodeI, node * nodeJ, section *sec):
	element2D(id, nodeI, nodeJ), ue(0.0), f(0.0)
{
	this->sec = sec;
	k = sec->mat->E*sec->A/L;
	T = rowvec( {-lxx, -lxy, lxx, lxy} );
	buildMatrix();
}

truss::~truss()
{
}

void truss::buildMatrix()
{
	K = T.t()*k*T;
}

void truss::getResponse(const bool update)
{
	u = vec( { nodeI->dofX->dsp, nodeI->dofY->dsp, nodeJ->dofX->dsp, nodeJ->dofY->dsp } );

	ue = 0.0;
	for (size_t i = 0; i < 4; i++)
	{
		ue += T(i)*u(i);
	}

	f = k * ue;

	force = &f;
	deformation = &ue;
}
