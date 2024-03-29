#pragma once

#include "DOF.h"
#include "ROD2D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The bilinear spring in X-Z plane.
 */
class SpringBilinear2D : public ROD2D
{
public:
	SpringBilinear2D(const int id, Node *nodeI, Node *nodeJ, const double k0, const double uy,
					const double alpha, RODS::LocalAxis axis);
	~SpringBilinear2D();

	void buildMatrix() override;
	void getResponse(const bool update = false) override;
	void assembleStiffnessMatrix(mat &K) override;
	void assembleNonlinearForceVector(vec &q) override;

	double k0, uy, k1, fy, alpha;
	double k, ue, f;
	double kp, up, dup, fp;
	size_t status, sp;
	RODS::LocalAxis axis;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K;
	vec::fixed<4> q;
};

