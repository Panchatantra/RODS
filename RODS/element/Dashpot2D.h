#pragma once

#include "DOF.h"
#include "Element2D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The linear dashpot in X-Z plane.
 */
class Dashpot2D : public Element2D
{
public:
	Dashpot2D(const int id, Node * nodeI, Node * nodeJ, double c, ELE::localAxis axis);
	~Dashpot2D();

	virtual void buildMatrix();
	virtual void assembleDampingMatrix(mat &C);
	virtual void getResponse(const bool update = false);

	double c;
	double ue, f;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> C;
};

