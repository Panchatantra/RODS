#include "Tri2D.h"

Tri2D::Tri2D(const int id, Node *nodeI, Node *nodeJ, Node *nodeP):
	Plane2D(id), nodeI(nodeI), nodeJ(nodeJ), nodeP(nodeP)
{
}

Tri2D::~Tri2D()
{
}