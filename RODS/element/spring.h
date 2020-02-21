#pragma once

#include "DOF.h"
#include "Element1D.h"
#include <armadillo>

using namespace arma;

class spring : public Element1D
{
public:
	spring(const int id, DOF *i, DOF *j, const double k);
	~spring();

	void buildMatrix();
	void assembleStiffnessMatrix(mat &K);
	void getResponse(const bool update = false);

	double k;
	double u, f;

	mat::fixed<2,2> K;
};

