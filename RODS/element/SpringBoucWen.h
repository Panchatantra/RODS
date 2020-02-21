#pragma once

#include "DOF.h"
#include "Element1D.h"
#include <armadillo>

using namespace arma;

class SpringBoucWen : public Element1D
{
public:
	SpringBoucWen(const int id, DOF *i, DOF *j, const double k0, const double uy, const double alpha, const double beta=0.5, const double n=20);
	~SpringBoucWen();

	void buildMatrix();
	void assembleStiffnessMatrix(mat &K);
	void getResponse(const bool update=false);

	void assembleNonlinearForceVector(vec &q);

	double k0, uy, k1, fy, alpha, beta, n;
	double k, u, f, v, z, zp;

	mat::fixed<2, 2> K;
	vec::fixed<2> q;
};

