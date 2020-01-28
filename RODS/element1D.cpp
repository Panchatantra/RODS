#include "element1D.h"


element1D::element1D()
{
	nv = 1;
}

element1D::element1D(const int id, dof * i, dof * j) : element(id), dofI(i), dofJ(j)
{
	nv = 1;
}


element1D::~element1D()
{
}
