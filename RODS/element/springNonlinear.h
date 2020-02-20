#pragma once

#include "DOF.h"
#include "element/element1D.h"
#include "material/material1D.h"
#include <armadillo>

using namespace arma;

class springNonlinear : public element1D
{
public:
	springNonlinear(const int id, DOF *i, DOF *j, material1D *smat);
	~springNonlinear();

	void buildMatrix();
	void assembleStiffnessMatrix(mat &K);
	void getResponse(const bool update=false);

	void assembleNonlinearForceVector(vec &q);

	material1D *smat;
	double u, f;

	mat::fixed<2, 2> K;
	vec::fixed<2> q;
};

