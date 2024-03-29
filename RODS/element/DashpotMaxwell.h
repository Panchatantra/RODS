#pragma once

#include "DOF.h"
#include "ROD1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The Maxwell dashpot in X direction.
 */
class DashpotMaxwell : public ROD1D
{
public:
	DashpotMaxwell(const int id, DOF *i, DOF *j, const double ks, const double c, const double alpha=1.0);
	~DashpotMaxwell();

	void getResponse(const bool update=false);
	void assembleNonlinearForceVector(vec &q);

	double ks, c, alpha;
	double u, f, ud, udp;

	vec::fixed<2> q;
};

