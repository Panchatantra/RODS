#include "Element2D.h"

Element2D::Element2D(const int id, Node * nodeI, Node * nodeJ):
	Element(id), nodeI(nodeI), nodeJ(nodeJ)
{
	double dx = nodeJ->x0 - nodeI->x0;
	double dz = nodeJ->z0 - nodeI->z0;
	L = sqrt(dx * dx + dz * dz);

	lxx = dx/L;
	lxz = dz/L;
	lzx = -lxz;
	lzz = lxx;
}

Element2D::~Element2D()
{
}
