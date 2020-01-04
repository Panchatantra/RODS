#pragma once

#include "dof.h"
#include "element1D.h"
#include <armadillo>

using namespace arma;

class dashpot : public element1D
{
public:
	dashpot(const int n, dof *i, dof *j, const double c);
	~dashpot();

	void buildMatrix();

	double damping;

	mat::fixed<2, 2> C;

	double force;
	
};

