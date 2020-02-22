#pragma once

#include "DOF.h"
#include "Element1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The linear dashpot in X direction.
 */
class Dashpot : public Element1D
{
public:
	Dashpot(const int id, DOF *i, DOF *j, const double c);
	~Dashpot();

	void buildMatrix();
	void assembleDampingMatrix(mat & C);
	void getResponse(const bool update = false);

	double c;
	double u, f;

	mat::fixed<2, 2> C;
};

