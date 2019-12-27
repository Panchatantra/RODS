#pragma once

#include "dof.h"
#include <armadillo>

using namespace arma;

class spring
{
public:
	spring(const int n, dof *i, dof *j);
	spring(const int n, dof *i, dof *j, const double k);
	~spring();

	void buildMatrix();

	int id;
	dof *dofI;
	dof *dofJ;
	double stiff;

	mat::fixed<2,2> K;

	double force;
};

