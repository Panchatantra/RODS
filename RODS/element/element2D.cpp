#include "element2D.h"

element2D::element2D(const int id, node * nodeI, node * nodeJ):
	element(id), nodeI(nodeI), nodeJ(nodeJ)
{
}

element2D::~element2D()
{
}
