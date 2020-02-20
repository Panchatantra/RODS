#pragma once

#include "DOF.h"
#include "element2D.h"
#include <armadillo>

using namespace arma;

class dashpotExp2D : public element2D
{
public:
	dashpotExp2D(const int id, node * nodeI, node * nodeJ, const double c, const double alpha,
		ELE::localAxis axis);
	~dashpotExp2D();

	virtual void buildMatrix();
	virtual void getResponse(const bool update = false);
	virtual void assembleNonlinearForceVector(vec &q);

	double c, alpha;
	double ue, f;

	rowvec::fixed<4> T;
	vec::fixed<4> q;
};

