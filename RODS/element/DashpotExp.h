#pragma once

#include "DOF.h"
#include "Element1D.h"
#include <armadillo>

using namespace arma;

class DashpotExp : public Element1D
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