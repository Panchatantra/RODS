#pragma once

#include "DOF.h"
#include "Element2D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The exponential nonlinear dashpot in X-Z plane.
 */
class DashpotExp2D : public Element2D
{
public:
	DashpotExp2D(const int id, Node * nodeI, Node * nodeJ, const double c, const double alpha,
		RODS::LocalAxis axis);
	~DashpotExp2D();

	virtual void buildMatrix();
	virtual void getResponse(const bool update = false);
	virtual void assembleNonlinearForceVector(vec &q);

	double c, alpha;
	double ue, f;

	rowvec::fixed<4> T;
	vec::fixed<4> q;
};

