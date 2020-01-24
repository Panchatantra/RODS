#include "spring2D.h"


spring2D::spring2D(const int id, node * nodeI, node * nodeJ, double k1, double k2)
{
	this->id = id;
	this->nodeI = nodeI;
	this->nodeJ = nodeJ;
	this->k1 = k1;
	this->k2 = k2;

	double dx = nodeJ->x - nodeI->x;
	double dy = nodeJ->y - nodeI->y;
	L = sqrt(dx*dx + dy*dy);

	lxx = dx / L;
	lxy = dy / L;
	lyx = -lxy;
	lyy = lxx;

	T << -lxx << -lxy << lxx << lxy << endr
	  << -lyx << -lyy << lyx << lyy << endr;
}


spring2D::~spring2D()
{
}

void spring2D::buildMatrix()
{
	D << k1 << 0 << endr
	  << 0 << k2 << endr;
	K = T.t()*D*T;
}

void spring2D::getResponse()
{
}
