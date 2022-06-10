#pragma once

#include "DOF.h"
#include "ROD3D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The Maxwell dashpot in 3D space.
 */
class DashpotMaxwell3D : public ROD3D
{
public:
	DashpotMaxwell3D(const int id, Node * nodeI, Node * nodeJ, const double k, const double c, const double alpha,
		RODS::LocalAxis axis);
	~DashpotMaxwell3D();

	virtual void getResponse(const bool update = false);
	virtual void assembleNonlinearForceVector(vec &q);

	double k, c, alpha;
	double ue, f, ud, udp;

	const size_t nv = 6;

	rowvec::fixed<6> T;
	vec::fixed<6> q;
};

