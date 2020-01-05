#include "node.h"


node::node(const int id, const double x, const double y, const double z)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->z = z;

	this->x0 = x;
	this->y0 = y;
	this->z0 = z;

	dofX = nullptr;
	dofY = nullptr;
	dofZ = nullptr;
	dofRX = nullptr;
	dofRY = nullptr;
	dofRZ = nullptr;
}


node::~node()
{
}

void node::setDof(dof * d)
{
	direction dir = d->dir;
	switch (dir)
	{
	case X:
        this->dofX = d;
		break;
	case Y:
        this->dofY = d;
		break;
	case Z:
        this->dofZ = d;
		break;
	case RX:
        this->dofRX = d;
		break;
	case RY:
        this->dofRY = d;
		break;
	case RZ:
        this->dofRZ = d;
		break;
	default:
		break;
	}
}
