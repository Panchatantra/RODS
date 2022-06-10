#include "Shell3D.h"

Shell3D::Shell3D(const int id, Node *nodeI, Node *nodeJ, Node *nodeP, Node *nodeQ) :
	Element(id), nodeI(nodeI), nodeJ(nodeJ), nodeP(nodeP), nodeQ(nodeQ)
{
}

Shell3D::~Shell3D()
{
}