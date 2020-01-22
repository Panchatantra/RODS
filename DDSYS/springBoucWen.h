#pragma once

#include "dof.h"
#include "element1D.h"
#include <armadillo>

using namespace arma;

class springBoucWen : public element1D
{
public:
	springBoucWen(const int id, dof *i, dof *j, const double k0, const double uy, const double alpha, const double beta=0.5, const double n=20);
	~springBoucWen();

	void buildMatrix();
	void assembleStiffnessMatrix(mat &K);
	void getResponse(const bool update=false);

	void assembleNonlinearForceVector(vec &q);

	double k0, uy, k1, fy, alpha;
	double k, u, f, v;
	double z, n, beta, zp;

	mat::fixed<2, 2> K;
	vec::fixed<2> q;
};

