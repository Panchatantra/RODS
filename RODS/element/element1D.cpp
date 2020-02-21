#include "Element1D.h"


Element1D::Element1D()
{
}

Element1D::Element1D(const int id, DOF * i, DOF * j) : Element(id), dofI(i), dofJ(j)
{
}


Element1D::~Element1D()
{
}
