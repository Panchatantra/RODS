#pragma once

#include "dof.h"
#include "element1D.h"
#include <armadillo>

using namespace arma;

class spring : public element1D
{
public:
	spring(const int n, dof *i, dof *j, const double k);
	~spring();

	void buildMatrix();

	double stiff;

	mat::fixed<2,2> K;

	double force;
};

