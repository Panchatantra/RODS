#pragma once

#include <map>
#include "dof.h"

class node
{
public:
	node(int id, double x=0, double y=0, double z=0);
	~node();

	void setDof(dof *d);
	bool isActivated(direction dir);

	int id;
	double x0, y0, z0;
	double x, y, z;

	dof *dofX, *dofY, *dofZ;
	dof *dofRX, *dofRY, *dofRZ;

};

