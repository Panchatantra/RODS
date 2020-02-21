#include "Plane2D.h"

Plane2D::Plane2D(const int id, Node * nodeI, Node * nodeJ, Node *nodeP, Node *nodeQ):
	Element(id), nodeI(nodeI), nodeJ(nodeJ), nodeP(nodeP), nodeQ(nodeQ)
{
}

Plane2D::~Plane2D()
{
}