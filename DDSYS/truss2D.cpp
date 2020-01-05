#include "truss2D.h"



truss2D::truss2D(const int id, node * nodeI, node * nodeJ, section *sec)
{
	this->id = id;
	this->nodeI = nodeI;
	this->nodeJ = nodeJ;
	this->sec = sec;

	double dx = nodeJ->x - nodeI->x;
	double dy = nodeJ->y - nodeI->y;
	L = sqrt(dx*dx+dy*dy);

	lxx = dx / L;
	lxy = dy / L;
}


truss2D::~truss2D()
{
}
