#pragma once

#include "DOF.h"
#include "ROD3D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The TVMD in 3D space.
 */
class TVMD3D : public ROD3D
{
public:
	TVMD3D(const int id, Node * nodeI, Node * nodeJ,
			const double m, const double c, const double k, RODS::LocalAxis axis);
	~TVMD3D();

	virtual void getResponse(const bool update = false);
	void assembleNonlinearForceVector(vec &q);

	rowvec::fixed<6> T;
	vec::fixed<6> q;

	double m, c, k;
	double ue, f;
	double ud, vd, udp, vdp;
};

