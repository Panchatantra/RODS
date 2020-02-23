#pragma once

#include "DOF.h"
#include "Element1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The inerter in X direction.
 */
class Inerter : public Element1D
{
public:
	Inerter(const int id, DOF *i, DOF *j, const double m);
	~Inerter();

	void buildMatrix();
	void assembleMassMatrix(mat & M);
	void getResponse(const bool update = false);

	double m;

	mat::fixed<2,2> M;

	double u, f;
};

