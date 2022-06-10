#pragma once

#include "ROD2D.h"
#include "material/Material1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The nonlinear spring related to a nonlinear Material1D in X-Z plane.
 */
class SpringNonlinear2D : public ROD2D
{
public:
	SpringNonlinear2D(const int id, Node * nodeI, Node * nodeJ, Material1D *smat, RODS::LocalAxis axis);
	~SpringNonlinear2D();

	void buildMatrix() override;
	void getResponse(const bool update = false) override;
	void assembleStiffnessMatrix(mat &K) override;
	void assembleNonlinearForceVector(vec &q) override;

	Material1D *smat;
	double k;
	double ue, f;
	RODS::LocalAxis axis;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K;
	vec::fixed<4> q;
};

