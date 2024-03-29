#pragma once

#include "DOF.h"
#include "ROD1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The serial-parallel inerter system in the X direction.
 */
class SPIS2 : public ROD1D
{
public:
	SPIS2(const int id, DOF *i, DOF *j, DOF *in, const double m, const double c, const double k);
	~SPIS2();

	void buildMatrix();
	void assembleMassMatrix(mat & M);
	void assembleStiffnessMatrix(mat & K);
	void assembleDampingMatrix(mat & C);
	void getResponse(const bool update = false);

	DOF *dofIN;
	double m, c, k;
	double *u, *f;
	mat::fixed<3,3> M, C, K;
};

