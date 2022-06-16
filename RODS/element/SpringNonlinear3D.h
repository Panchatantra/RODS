#pragma once

#include "ROD3D.h"
#include "material/Material1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The 3D bilinear spring.
 */
class SpringNonlinear3D : public ROD3D
{
public:
	SpringNonlinear3D(const int id, Node * nodeI, Node * nodeJ,
						Material1D *smat, RODS::LocalAxis axis);
	~SpringNonlinear3D();

	void buildMatrix() override;
	void getResponse(const bool update = false) override;
	void assembleStiffnessMatrix(mat &K) override;
	void assembleNonlinearForceVector(vec &q) override;

	Material1D *smat;
	double k;
	double ue, f;
	RODS::LocalAxis axis;

	rowvec::fixed<6> T;  ///< The geometry transformation matrix
	mat::fixed<6, 6> K;  ///< The stiffness matrix
	vec::fixed<6> q;     ///< The nonlinear force vector
};