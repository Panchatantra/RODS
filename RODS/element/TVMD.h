#pragma once

#include "DOF.h"
#include "Element1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The tuned viscous mass damper in X direction.
 */
class TVMD : public Element1D
{
public:
	TVMD(const int id, DOF *i, DOF *j, const double m, const double c, const double k);
	~TVMD();

	void getResponse(const bool update = false);
	void assembleNonlinearForceVector(vec &q);

	void func(const double uc, const double ud, const double vd, double &kud, double &kvd);

	double m, c, k;
	double u, f;
	double ud, vd, udp, vdp;

	vec::fixed<2> q;
};

