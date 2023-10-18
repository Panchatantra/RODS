#include "ROD3D.h"

ROD3D::ROD3D() :
	Element(), nodeI(nullptr), nodeJ(nullptr)
{
}

ROD3D::ROD3D(const int id, Node *nodeI, Node *nodeJ) :
	Element(id), nodeI(nodeI), nodeJ(nodeJ)
{
	double dx = nodeJ->x0 - nodeI->x0;
	double dy = nodeJ->y0 - nodeI->y0;
	double dz = nodeJ->z0 - nodeI->z0;
	L = sqrt(dx*dx + dy*dy + dz*dz);
	double l = sqrt(dx*dx + dy*dy);

	if (dx == 0.0 && dy == 0.0)
	{
		lxx = 0.0;
		lxy = 0.0;
		lxz = 1.0;
		lyx = 1.0;
		lyy = 0.0;
		lyz = 0.0;
		lzx = 0.0;
		lzy = 1.0;
		lzz = 0.0;
	}
	else
	{
		lxx = dx/L;
		lxy = dy/L;
		lxz = dz/L;
		lyx = -dx*dz/(L*l);
		lyy = dy*dz/(L*l);
		lyz = l/L;
		lzx = dy/l;
		lzy = -dx/l;
		lzz = 0.0;
	}
}

ROD3D::~ROD3D()
{
}
