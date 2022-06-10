#pragma once

#include "DOF.h"
#include "ROD2D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The inerter in X-Z plane.
 */
class Inerter2D : public ROD2D
{
public:
	Inerter2D(const int id, Node * nodeI, Node * nodeJ, double m, RODS::LocalAxis axis);
	~Inerter2D();

	virtual void buildMatrix();
	virtual void assembleMassMatrix(mat &M);
	virtual void getResponse(const bool update = false);

	double m;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> M;

	double ue, f;
};

