#pragma once

#include "DOF.h"
#include "element/Element1D.h"
#include "material/Material1D.h"
#include <armadillo>

using namespace arma;

class SpringNonlinear : public Element1D
{
public:
	SpringNonlinear(const int id, DOF *i, DOF *j, Material1D *smat);
	~SpringNonlinear();

	void buildMatrix();
	void assembleStiffnessMatrix(mat &K);
	void getResponse(const bool update=false);

	void assembleNonlinearForceVector(vec &q);

	Material1D *smat;
	double u, f;

	mat::fixed<2, 2> K;
	vec::fixed<2> q;
};

