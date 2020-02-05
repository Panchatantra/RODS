#pragma once

#include "basis.h"
#include "dof.h"

class node : public basis
{
public:
	node(const int id, const double x=0.0, const double y=0.0, const double z=0.0);
	~node();

	void setDof(dof *d);
	void fixDof();
	void fixDof(direction dir);
	bool isActivated(direction dir);

	double x0, y0, z0;
	double x, y, z;

	dof *dofX, *dofY, *dofZ;
	dof *dofRX, *dofRY, *dofRZ;

};

