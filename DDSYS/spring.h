#pragma once

#include "dof.h"
#include "element1D.h"
#include <armadillo>

using namespace arma;

class spring : public element1D
{
public:
	spring(const int id, dof *i, dof *j, const double k);
	~spring();

	void buildMatrix();
	void getResponse();

	double k;

	mat::fixed<2,2> K;

	double f;
};

