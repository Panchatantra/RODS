#pragma once

#include "DOF.h"
#include "Element2D.h"
#include <armadillo>

using namespace arma;

class dashpot2D : public Element2D
{
public:
	dashpot2D(const int id, Node * nodeI, Node * nodeJ, double c, ELE::localAxis axis);
	~dashpot2D();

	virtual void buildMatrix();
	virtual void assembleDampingMatrix(mat &C);
	virtual void getResponse(const bool update = false);

	double c;
	double ue, f;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> C;
};

