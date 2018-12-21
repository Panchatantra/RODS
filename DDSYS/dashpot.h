#pragma once

#include "dof.h"
#include <armadillo>

using namespace arma;

class dashpot
{
public:
	dashpot(const int n, dof *i, dof *j);
	dashpot(const int n, dof *i, dof *j, const double c);
	~dashpot();

	void buildMatrix();

	int num;
	dof *dofI;
	dof *dofJ;
	double damping;

	mat::fixed<2, 2> C;

	double force;
	
};

