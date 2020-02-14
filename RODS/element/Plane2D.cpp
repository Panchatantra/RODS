#include "Plane2D.h"

Plane2D::Plane2D(const int id, node * nodeI, node * nodeJ, node *nodeP, node *nodeQ):
	element(id), nodeI(nodeI), nodeJ(nodeJ), nodeP(nodeP), nodeQ(nodeQ)
{
}

Plane2D::~Plane2D()
{
}