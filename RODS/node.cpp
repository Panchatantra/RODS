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

void node::fixDof()
{
	if (dofX != nullptr) dofX->isFixed = true;
	if (dofY != nullptr) dofY->isFixed = true;
	if (dofZ != nullptr) dofZ->isFixed = true;
	if (dofRX != nullptr) dofRX->isFixed = true;
	if (dofRY != nullptr) dofRY->isFixed = true;
	if (dofRZ != nullptr) dofRZ->isFixed = true;
	
}

bool node::isActivated(direction dir)
{
	dof *d = nullptr;
	switch (dir)
	{
	case X:
		d = dofX;
		break;
	case Y:
		d = dofY;
		break;
	case Z:
		d = dofZ;
		break;
	case RX:
		d = dofRX;
		break;
	case RY:
		d = dofRY;
		break;
	case RZ:
		d = dofRZ;
		break;
	default:
		break;
	}

	if (d == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}
