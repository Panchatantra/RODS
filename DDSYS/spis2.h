#pragma once

#include "dof.h"
#include <armadillo>

using namespace arma;

class spis2
{
public:
	spis2(const int n, dof *i, dof *j, dof *in);
	spis2(const int n, dof *i, dof *j, dof *in, const double m, const double c, const double k);
	~spis2();

	void buildMatrix();

	int id;
	dof *dofI;
	dof *dofJ;
	dof *dofIN;
	double inertance;
	double damping;
	double stiff;

	mat M, C, K;

	double force;
};

