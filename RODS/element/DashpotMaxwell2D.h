#pragma once

#include "DOF.h"
#include "ROD2D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The Maxwell dashpot in X-Z plane.
 */
class DashpotMaxwell2D : public ROD2D
{
public:
	DashpotMaxwell2D(const int id, Node * nodeI, Node * nodeJ, const double k, const double c, const double alpha,
		RODS::LocalAxis axis);
	~DashpotMaxwell2D();

	virtual void getResponse(const bool update = false);
	virtual void assembleNonlinearForceVector(vec &q);

	double k, c, alpha;
	double ue, f, ud, udp;

	const size_t nv = 4;

	rowvec::fixed<4> T;
	vec::fixed<4> q;
};

