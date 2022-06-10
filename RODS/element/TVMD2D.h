#pragma once

#include "DOF.h"
#include "ROD2D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The TVMD in X-Z plane.
 */
class TVMD2D : public ROD2D
{
public:
	TVMD2D(const int id, Node * nodeI, Node * nodeJ, const double m, const double c, const double k, RODS::LocalAxis axis);
	~TVMD2D();

	virtual void getResponse(const bool update = false);
	void assembleNonlinearForceVector(vec &q);

	rowvec::fixed<4> T;
	vec::fixed<4> q;

	double m, c, k;
	double ue, f;
	double ud, vd, udp, vdp;
};

