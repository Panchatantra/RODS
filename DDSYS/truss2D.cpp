#include "truss2D.h"

truss2D::truss2D(const int id, node * nodeI, node * nodeJ, section *sec)
{
	this->id = id;
	this->nodeI = nodeI;
	this->nodeJ = nodeJ;
	this->sec = sec;

	double dx = nodeJ->x - nodeI->x;
	double dy = nodeJ->y - nodeI->y;
	L = sqrt(dx*dx + dy*dy);

	k = sec->mat->E0*sec->A / L;

	lxx = dx / L;
	lxy = dy / L;

	T = rowvec( {-lxx, -lxy, lxx, lxy} );
}


truss2D::~truss2D()
{
}

void truss2D::buildMatrix()
{
	K = T.t()*k*T;
}

void truss2D::getResponse()
{
	u = vec( { nodeI->dofX->dsp, nodeI->dofY->dsp, nodeJ->dofX->dsp, nodeJ->dofY->dsp } );

	ue = 0.0;

	for (size_t i = 0; i < 4; i++)
	{
		ue += T(i)*u(i);
	}

	f = k * ue;
}
