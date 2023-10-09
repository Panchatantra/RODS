#include "ROD2D.h"

ROD2D::ROD2D():
	Element(0), nodeI(nullptr), nodeJ(nullptr),
	L(0.0), lxx(0.0), lxz(0.0), lzx(0.0), lzz(0.0),
	axis(RODS::LocalAxis::U1)
{
}

ROD2D::ROD2D(const int id, Node * nodeI, Node * nodeJ):
	Element(id), nodeI(nodeI), nodeJ(nodeJ),
	axis(RODS::LocalAxis::U1)
{
	double dx = nodeJ->x0 - nodeI->x0;
	double dz = nodeJ->z0 - nodeI->z0;
	L = sqrt(dx * dx + dz * dz);

	lxx = dx/L;
	lxz = dz/L;
	lzx = -lxz;
	lzz = lxx;
}

ROD2D::ROD2D(const int id, Node *nodeI, Node *nodeJ, RODS::LocalAxis axis) :
	Element(id), nodeI(nodeI), nodeJ(nodeJ),
	axis(axis)
{
	double dx = nodeJ->x0 - nodeI->x0;
	double dz = nodeJ->z0 - nodeI->z0;
	L = sqrt(dx * dx + dz * dz);

	lxx = dx/L;
	lxz = dz/L;
	lzx = -lxz;
	lzz = lxx;
}

ROD2D::~ROD2D()
{
}
