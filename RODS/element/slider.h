#pragma once

#include "dof.h"
#include "element1D.h"
#include <armadillo>

using namespace arma;

class slider : public element1D
{
public:
	slider(const int id, dof *i, dof *j, const double muN);
	~slider();

	void buildMatrix();
	void assembleStiffnessMatrix(mat &K);
	void getResponse(const bool update=false);
	void assembleNonlinearForceVector(vec &q);

	double muN;
	double k, u, f, v;

	mat::fixed<2, 2> K;
	vec::fixed<2> q;
};

