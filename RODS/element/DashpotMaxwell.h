#pragma once

#include "DOF.h"
#include "Element1D.h"
#include <armadillo>

using namespace arma;

class DashpotMaxwell : public Element1D
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

