#include "Node.h"


Node::Node(const int id, const double x, const double y, const double z):
		Basis(id), x(x), y(y), z(z), x0(x), y0(y), z0(z),
		dofX(nullptr), dofY(nullptr), dofZ(nullptr),
		dofRX(nullptr), dofRY(nullptr), dofRZ(nullptr)
{
}


Node::~Node()
{
}

void Node::setDof(DOF * d)
{
	Direction dir = d->dir;
	switch (dir)
	{
	case Direction::X:
        this->dofX = d;
		break;
	case Direction::Y:
        this->dofY = d;
		break;
	case Direction::Z:
        this->dofZ = d;
		break;
	case Direction::RX:
        this->dofRX = d;
		break;
	case Direction::RY:
        this->dofRY = d;
		break;
	case Direction::RZ:
        this->dofRZ = d;
		break;
	default:
		break;
	}
}

void Node::fixDof()
{
	if (dofX != nullptr) dofX->isFixed = true;
	if (dofY != nullptr) dofY->isFixed = true;
	if (dofZ != nullptr) dofZ->isFixed = true;
	if (dofRX != nullptr) dofRX->isFixed = true;
	if (dofRY != nullptr) dofRY->isFixed = true;
	if (dofRZ != nullptr) dofRZ->isFixed = true;
}

void Node::fixDof(Direction dir)
{
	if (isActivated(dir))
	{
		switch (dir)
		{
		case Direction::X:
			dofX->isFixed = true;
			break;
		case Direction::Y:
			dofY->isFixed = true;
			break;
		case Direction::Z:
			dofZ->isFixed = true;
			break;
		case Direction::RX:
			dofRX->isFixed = true;
			break;
		case Direction::RY:
			dofRY->isFixed = true;
			break;
		case Direction::RZ:
			dofRZ->isFixed = true;
			break;
		default:
			break;
		}
	}
}

bool Node::isActivated(Direction dir)
{
	DOF *d = nullptr;
	switch (dir)
	{
	case Direction::X:
		d = dofX;
		break;
	case Direction::Y:
		d = dofY;
		break;
	case Direction::Z:
		d = dofZ;
		break;
	case Direction::RX:
		d = dofRX;
		break;
	case Direction::RY:
		d = dofRY;
		break;
	case Direction::RZ:
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
