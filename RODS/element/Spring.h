#pragma once

#include "DOF.h"
#include "ROD1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The spring in X direction.
 */
class Spring : public ROD1D
{
public:
	Spring(const int id, DOF *i, DOF *j, const double k);
	~Spring();

	void buildMatrix();
	void assembleStiffnessMatrix(mat &K);
	void getResponse(const bool update = false);

	double k;
	double u, f;

	mat::fixed<2,2> K;
};

