#include "Element2D.h"

Element2D::Element2D(const int id, Node * nodeI, Node * nodeJ):
	Element(id), nodeI(nodeI), nodeJ(nodeJ)
{
	double dx = nodeJ->x0 - nodeI->x0;
	double dy = nodeJ->z0 - nodeI->z0;
	L = sqrt(dx * dx + dy * dy);

	lxx = dx/L;
	lxy = dy/L;
	lyx = -lxy;
	lyy = lxx;
}

Element2D::~Element2D()
{
}
