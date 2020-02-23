#pragma once

#include "DOF.h"
#include "Element2D.h"
#include "material/Material1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The nonlinear spring related to a nonlinear Material1D in X-Z plane.
 */
class SpringNonlinear2D : public Element2D
{
public:
	SpringNonlinear2D(const int id, Node * nodeI, Node * nodeJ, Material1D *smat, ELE::localAxis axis);
	~SpringNonlinear2D();

	void buildMatrix() override;
	void getResponse(const bool update = false) override;
	void assembleStiffnessMatrix(mat &K) override;
	void assembleNonlinearForceVector(vec &q) override;

	Material1D *smat;
	double k;
	double ue, f;
	ELE::localAxis axis;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K;
	vec::fixed<4> q;
};

