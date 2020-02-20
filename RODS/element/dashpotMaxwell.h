#pragma once

#include "DOF.h"
#include "element1D.h"
#include <armadillo>

using namespace arma;

class dashpotMaxwell : public element1D
{
public:
	dashpotMaxwell(const int id, DOF *i, DOF *j, const double ks, const double c, const double alpha=1.0);
	~dashpotMaxwell();

	void getResponse(const bool update=false);
	void assembleNonlinearForceVector(vec &q);

	double ks, c, alpha;
	double u, f, ud, udp;

	vec::fixed<2> q;
};

