#pragma once

#include "Element2D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The Bouc-Wen spring in X-Z plane.
 */
class SpringBoucWen2D : public Element2D
{
public:
	SpringBoucWen2D(const int id, Node * nodeI, Node * nodeJ, const double k0, const double uy,
		const double alpha, const double beta, const double n, RODS::LocalAxis axis);
	~SpringBoucWen2D();

	virtual void buildMatrix();
	virtual void getResponse(const bool update = false);
	virtual void assembleStiffnessMatrix(mat &K);
	virtual void assembleNonlinearForceVector(vec &q);

	double k0, uy, fy, alpha, beta, n;
	double k, ue, f, ve, z, zp;
	RODS::LocalAxis axis;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K;
	vec::fixed<4> q;
};

