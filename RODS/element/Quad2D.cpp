#include "Quad2D.h"

Quad2D::Quad2D(const int id, Node *nodeI, Node *nodeJ, Node *nodeP, Node *nodeQ):
	Plane2D(id), nodeI(nodeI), nodeJ(nodeJ), nodeP(nodeP), nodeQ(nodeQ)
{
}

Quad2D::~Quad2D()
{
}