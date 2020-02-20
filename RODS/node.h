#pragma once

#include "Basis.h"
#include "DOF.h"

class node : public Basis
{
public:
	node(const int id, const double x=0.0, const double y=0.0, const double z=0.0);
	~node();

	void setDof(DOF *d);
	void fixDof();
	void fixDof(direction dir);
	bool isActivated(direction dir);

	double x0, y0, z0;
	double x, y, z;

	DOF *dofX, *dofY, *dofZ;
	DOF *dofRX, *dofRY, *dofRZ;

};

