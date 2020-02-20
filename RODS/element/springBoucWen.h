#pragma once

#include "DOF.h"
#include "element1D.h"
#include <armadillo>

using namespace arma;

class springBoucWen : public element1D
{
public:
	springBoucWen(const int id, DOF *i, DOF *j, const double k0, const double uy, const double alpha, const double beta=0.5, const double n=20);
	~springBoucWen();

	void buildMatrix();
	void assembleStiffnessMatrix(mat &K);
	void getResponse(const bool update=false);

	void assembleNonlinearForceVector(vec &q);

	double k0, uy, k1, fy, alpha, beta, n;
	double k, u, f, v, z, zp;

	mat::fixed<2, 2> K;
	vec::fixed<2> q;
};

