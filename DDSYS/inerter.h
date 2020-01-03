#pragma once

#include "dof.h"
#include "element1D.h"
#include <armadillo>

using namespace arma;

class inerter : public element1D
{
public:
	inerter(const int n, dof *i, dof *j);
	inerter(const int n, dof *i, dof *j, const double m);
	~inerter();

	void buildMatrix();

	int id;
	dof *dofI;
	dof *dofJ;
	double inertance;

	mat::fixed<2,2> M;

	double force;
};

