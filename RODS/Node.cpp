#include "Node.h"

Node::Node() :
	Basis(0), x(0.0), y(0.0), z(0.0), x0(0.0), y0(0.0), z0(0.0),
	dofX(nullptr), dofY(nullptr), dofZ(nullptr),
	dofRX(nullptr), dofRY(nullptr), dofRZ(nullptr),
	dim(RODS::Dimension::ONE)
{
}

Node::Node(const int id, const double x, const double y, const double z) :
	Basis(id), x(x), y(y), z(z), x0(x), y0(y), z0(z),
	dofX(nullptr), dofY(nullptr), dofZ(nullptr),
	dofRX(nullptr), dofRY(nullptr), dofRZ(nullptr),
	dim(RODS::Dimension::ONE)
{
}


Node::~Node()
{
}

void Node::setDof(DOF * d)
{
	RODS::Direction dir = d->dir;
	switch (dir)
	{
	case RODS::Direction::X:
        this->dofX = d;
		break;
	case RODS::Direction::Y:
        this->dofY = d;
		break;
	case RODS::Direction::Z:
        this->dofZ = d;
		break;
	case RODS::Direction::RX:
        this->dofRX = d;
		break;
	case RODS::Direction::RY:
        this->dofRY = d;
		break;
	case RODS::Direction::RZ:
        this->dofRZ = d;
		break;
	default:
		break;
	}
}

void Node::fixDOF()
{
	if (dofX != nullptr) dofX->isFixed = true;
	if (dofY != nullptr) dofY->isFixed = true;
	if (dofZ != nullptr) dofZ->isFixed = true;
	if (dofRX != nullptr) dofRX->isFixed = true;
	if (dofRY != nullptr) dofRY->isFixed = true;
	if (dofRZ != nullptr) dofRZ->isFixed = true;
}

void Node::fixDOF(RODS::Direction dir)
{
	if (isActivated(dir))
	{
		switch (dir)
		{
		case RODS::Direction::X:
			dofX->isFixed = true;
			break;
		case RODS::Direction::Y:
			dofY->isFixed = true;
			break;
		case RODS::Direction::Z:
			dofZ->isFixed = true;
			break;
		case RODS::Direction::RX:
			dofRX->isFixed = true;
			break;
		case RODS::Direction::RY:
			dofRY->isFixed = true;
			break;
		case RODS::Direction::RZ:
			dofRZ->isFixed = true;
			break;
		default:
			break;
		}
	}
}

bool Node::isActivated(RODS::Direction dir)
{
	DOF *d = nullptr;
	switch (dir)
	{
	case RODS::Direction::X:
		d = dofX;
		break;
	case RODS::Direction::Y:
		d = dofY;
		break;
	case RODS::Direction::Z:
		d = dofZ;
		break;
	case RODS::Direction::RX:
		d = dofRX;
		break;
	case RODS::Direction::RY:
		d = dofRY;
		break;
	case RODS::Direction::RZ:
		d = dofRZ;
		break;
	default:
		break;
	}

	if (d == nullptr) return false;
	else return true;

}

void Node::Deactivate(RODS::Direction dir)
{
	switch (dir)
	{
	case RODS::Direction::X:
		dofX = nullptr;
		break;
	case RODS::Direction::Y:
		dofY = nullptr;
		break;
	case RODS::Direction::Z:
		dofZ = nullptr;
		break;
	case RODS::Direction::RX:
		dofRX = nullptr;
		break;
	case RODS::Direction::RY:
		dofRY = nullptr;
		break;
	case RODS::Direction::RZ:
		dofRZ = nullptr;
		break;
	default:
		break;
	}
}

void Node::setDim(RODS::Dimension dim)
{
	this->dim = dim;
}

void Node::setMass(const double m)
{
	if (dofX != nullptr) dofX->setMass(m);
	if (dofY != nullptr) dofY->setMass(m);
	if (dofZ != nullptr) dofZ->setMass(m);
	if (dofRX != nullptr) dofRX->setMass(m);
	if (dofRY != nullptr) dofRY->setMass(m);
	if (dofRZ != nullptr) dofRZ->setMass(m);
}

void Node::setMass(const double m, RODS::Direction dir)
{
	if (isActivated(dir))
	{
		switch (dir)
		{
		case RODS::Direction::X:
			dofX->setMass(m);
			break;
		case RODS::Direction::Y:
			dofY->setMass(m);
			break;
		case RODS::Direction::Z:
			dofZ->setMass(m);
			break;
		case RODS::Direction::RX:
			dofRX->setMass(m);
			break;
		case RODS::Direction::RY:
			dofRY->setMass(m);
			break;
		case RODS::Direction::RZ:
			dofRZ->setMass(m);
			break;
		default:
			break;
		}
	}
}

void Node::setMass(const double m, const double I)
{
	if (dofX != nullptr) dofX->setMass(m);
	if (dofY != nullptr) dofY->setMass(m);
	if (dofZ != nullptr) dofZ->setMass(m);
	if (dofRX != nullptr) dofRX->setMass(I);
	if (dofRY != nullptr) dofRY->setMass(I);
	if (dofRZ != nullptr) dofRZ->setMass(I);
}

int Node::getIdDof(RODS::Direction dir)
{
	switch (dir)
	{
	case RODS::Direction::X:
		return dofX->id;
		break;
	case RODS::Direction::Y:
		return dofY->id;
		break;
	case RODS::Direction::Z:
		return dofZ->id;
		break;
	case RODS::Direction::RX:
		return dofRX->id;
		break;
	case RODS::Direction::RY:
		return dofRY->id;
		break;
	case RODS::Direction::RZ:
		return dofRZ->id;
		break;
	default:
		break;
	}
}
