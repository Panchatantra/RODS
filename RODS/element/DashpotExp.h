#pragma once

#include "DOF.h"
#include "ROD1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The exponential nonlinear dashpot in X direction.
 */
class DashpotExp : public ROD1D
{
public:
	DashpotExp(const int id, DOF *i, DOF *j, const double c, const double alpha);
	~DashpotExp();

	void buildMatrix();
	void assembleDampingMatrix(mat & C);

	void getResponse(const bool update = false);
	void assembleNonlinearForceVector(vec &q);

	double c, alpha;
	double u, f;

	mat::fixed<2, 2> C;
	vec::fixed<2> q;
};