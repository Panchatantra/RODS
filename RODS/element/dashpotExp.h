#pragma once

#include "dof.h"
#include "element1D.h"
#include <armadillo>

using namespace arma;

class dashpotExp : public element1D
{
public:
	dashpotExp(const int id, dof *i, dof *j, const double c, const double alpha);
	~dashpotExp();

	void buildMatrix();
	void assembleDampingMatrix(mat & C);

	void getResponse(const bool update = false);
	void assembleNonlinearForceVector(vec &q);

	double c, alpha;
	double u, f;

	mat::fixed<2, 2> C;
	vec::fixed<2> q;
};