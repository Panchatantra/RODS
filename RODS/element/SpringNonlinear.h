#pragma once

#include "DOF.h"
#include "element/ROD1D.h"
#include "material/Material1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The nonlinear spring related to a nonlinear Material1D in X direction.
 */
class SpringNonlinear : public ROD1D
{
public:
	SpringNonlinear(const int id, DOF *i, DOF *j, Material1D *smat);
	~SpringNonlinear();

	void buildMatrix();
	void assembleStiffnessMatrix(mat &K);
	void getResponse(const bool update=false);
	void assembleNonlinearForceVector(vec &q);

	Material1D *smat;
	double k;
	double u, f;

	mat::fixed<2, 2> K;
	vec::fixed<2> q;
};

