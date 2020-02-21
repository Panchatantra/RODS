#pragma once

#include "DOF.h"
#include "Element2D.h"
#include <armadillo>

using namespace arma;

class inerter2D : public Element2D
{
public:
	inerter2D(const int id, Node * nodeI, Node * nodeJ, double m, ELE::localAxis axis);
	~inerter2D();

	virtual void buildMatrix();
	virtual void assembleMassMatrix(mat &M);
	virtual void getResponse(const bool update = false);

	double m;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> M;

	double ue, f;
};

