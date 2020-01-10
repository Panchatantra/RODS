#pragma once

#include "dof.h"
#include "element1D.h"
#include <armadillo>

using namespace arma;

class springBilinear : public element1D
{
public:
	springBilinear(const int id, dof *i, dof *j, const double k0, const double uy, const double alpha);
	~springBilinear();

	void buildMatrix();
	void assembleStiffnessMatrix(mat &K);
	void getResponse(const bool update=false);

	double k0, uy, k1, fy;
	double k, u, f;
	double kp, up, dup, fp;
	size_t status, sp;

	mat::fixed<2, 2> K;
	vec::fixed<2> q;
};

