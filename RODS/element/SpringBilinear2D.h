#pragma once

#include "DOF.h"
#include "Element2D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The bilinear spring in X-Z plane.
 */
class SpringBilinear2D : public Element2D
{
public:
	SpringBilinear2D(const int id, DOF *i, DOF *j, const double k0, const double uy,
					const double alpha, ELE::LocalAxis axis);
	~SpringBilinear2D();

	void buildMatrix() override;
	void getResponse(const bool update = false) override;
	void assembleStiffnessMatrix(mat &K) override;
	void assembleNonlinearForceVector(vec &q) override;

	double k0, uy, k1, fy, alpha;
	double k, ue, f;
	double kp, up, dup, fp;
	size_t status, sp;
	ELE::LocalAxis axis;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K;
	vec::fixed<4> q;
};

