#pragma once

#include "DOF.h"
#include "element2D.h"
#include <armadillo>

using namespace arma;

class dashpotMaxwell2D : public element2D
{
public:
	dashpotMaxwell2D(const int id, node * nodeI, node * nodeJ, const double k, const double c, const double alpha,
		ELE::localAxis axis);
	~dashpotMaxwell2D();

	virtual void getResponse(const bool update = false);
	virtual void assembleNonlinearForceVector(vec &q);

	double k, c, alpha;
	double ue, f, ud, udp;

	rowvec::fixed<4> T;
	vec::fixed<4> q;
};

