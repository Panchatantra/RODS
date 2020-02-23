#pragma once

#include "Basis.h"
#include "DOF.h"

/**
 * @brief      The node.
 */
class Node : public Basis
{
public:
	Node(const int id, const double x=0.0, const double y=0.0, const double z=0.0);
	~Node();

	void setDof(DOF *d);
	void fixDOF();
	void fixDOF(Direction dir);
	bool isActivated(Direction dir);

	double x, y, z;
	double x0, y0, z0;

	DOF *dofX, *dofY, *dofZ;
	DOF *dofRX, *dofRY, *dofRZ;

};

