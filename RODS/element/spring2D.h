#pragma once

#include "Element2D.h"
#include "Node.h"

class spring2D :
	public Element2D
{
public:
	spring2D(const int id, Node * nodeI, Node * nodeJ, const double k, ELE::localAxis axis);
	~spring2D();

	virtual void buildMatrix();
	virtual void getResponse();
	virtual void assembleStiffnessMatrix(mat &K);

	vec::fixed<4> u;

	double k;
	double ue, f;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K;
};

