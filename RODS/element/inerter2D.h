#pragma once

#include "dof.h"
#include "element2D.h"
#include <armadillo>

using namespace arma;

class inerter2D : public element2D
{
public:
	inerter2D(const int id, node * nodeI, node * nodeJ, double m, ELE::localAxis axis);
	~inerter2D();

	virtual void buildMatrix();
	virtual void assembleMassMatrix(mat & M);
	virtual void getResponse(const bool update = false);

	double m;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> M;

	double ue, f;
};

