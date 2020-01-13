#pragma once

#include "dof.h"
#include "element1D.h"
#include <armadillo>

using namespace arma;

class springBoucWen : public element1D
{
public:
	springBoucWen(const int id, dof *i, dof *j, const double k0, const double uy, const double alpha);
	~springBoucWen();

	void buildMatrix();
	void assembleStiffnessMatrix(mat &K);
	void getResponse(const bool update=false);

	void assembleNonlinearForceVector(vec &q);

	double k0, uy, k1, fy, alpha;
	double k, u, f;
	double kp, up, dup, fp;
	size_t status, sp;

	mat::fixed<2, 2> K;
	vec::fixed<2> q;
};
