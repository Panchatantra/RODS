#pragma once

#include "Basis.h"
#include "DOF.h"

namespace RODS {
	enum class Dimension
	{
		ONE = 1,
		TWO,
		THREE,
		TWO_WITHOUT_ROTATE = 20,
		THREE_WITHOUT_ROTATE = 30,
	};
}

/**
 * @brief      The node.
 */
class Node : public Basis
{
public:
	Node();
	Node(const int id, const double x=0.0, const double y=0.0, const double z=0.0);
	~Node();

	void setDof(DOF *d);
	void fixDOF();
	void fixDOF(RODS::Direction dir);
	bool isActivated(RODS::Direction dir);
	void Deactivate(RODS::Direction dir);

	void setMass(const double m);
	void setMass(const double m, RODS::Direction dir);

	double x, y, z;
	double x0, y0, z0;

	DOF *dofX, *dofY, *dofZ;
	DOF *dofRX, *dofRY, *dofRZ;
};

