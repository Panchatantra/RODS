#pragma once

#include "ROD3D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The 3D bilinear spring.
 */
class SpringBilinear3D : public ROD3D
{
public:
	SpringBilinear3D(const int id, Node * nodeI, Node * nodeJ, const double k0, const double uy,
					const double alpha, RODS::LocalAxis axis);
	~SpringBilinear3D();

	void buildMatrix() override;
	void getResponse(const bool update = false) override;
	void assembleStiffnessMatrix(mat &K) override;
	void assembleNonlinearForceVector(vec &q) override;

	double k0;    ///< The initial stiffness
	double uy;    ///< The yield deformation
	double k1;    ///< The post-yield stiffness
	double fy;    ///< The yield force
	double alpha; ///< The post-yield stiffness ratio
	double k;     ///< The tangent stiffness
	double ue;    ///< The deformation
	double f;     ///< The force
	double kp;    ///< The tangent stiffness of previous step
	double up;    ///< The deformation of previous step
	double dup;   ///< The deformation variation of previous step
	double fp;    ///< The force of previous step
	size_t status;///< The status
	size_t sp;    ///< The status of previous step
	RODS::LocalAxis axis;    ///< The deform direction

	rowvec::fixed<6> T;  ///< The geometry transformation matrix
	mat::fixed<6, 6> K;  ///< The stiffness matrix
	vec::fixed<6> q;     ///< The nonlinear force vector
};