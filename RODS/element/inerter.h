#pragma once

#include "DOF.h"
#include "Element1D.h"
#include <armadillo>

using namespace arma;

class inerter : public Element1D
{
public:
	inerter(const int id, DOF *i, DOF *j, const double m);
	~inerter();

	void buildMatrix();
	void assembleMassMatrix(mat & M);
	void getResponse(const bool update = false);

	double m;

	mat::fixed<2,2> M;

	double u, f;
};

