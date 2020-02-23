#pragma once

#include "DOF.h"
#include "Element1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The bilinear spring in X direction.
 */
class SpringBilinear : public Element1D
{
public:
	SpringBilinear(const int id, DOF *i, DOF *j, const double k0, const double uy, const double alpha);
	~SpringBilinear();

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

