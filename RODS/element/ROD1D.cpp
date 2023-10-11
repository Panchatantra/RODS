#include "ROD1D.h"


ROD1D::ROD1D() :
	Element(0), dofI(nullptr), dofJ(nullptr)
{
}

ROD1D::ROD1D(const int id, DOF * i, DOF * j) :
	Element(id), dofI(i), dofJ(j)
{
}


ROD1D::~ROD1D()
{
}
