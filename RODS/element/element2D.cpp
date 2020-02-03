#include "element2D.h"

element2D::element2D(const int id, node * nodeI, node * nodeJ):
	element(id), nodeI(nodeI), nodeJ(nodeJ)
{
	double dx = nodeJ->x - nodeI->x;
	double dy = nodeJ->y - nodeI->y;
	L = sqrt(dx * dx + dy * dy);

	lxx = dx / L;
	lxy = dy / L;
	lyx = -lxy;
	lyy = lxx;
}

element2D::~element2D()
{
}
