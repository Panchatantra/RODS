#pragma once

#include "element2D.h"
#include "node.h"

class spring2D :
	public element2D
{
public:
	spring2D(const int id, node * nodeI, node * nodeJ, const double k, ELE::localAxis axis);
	~spring2D();

	virtual void buildMatrix();
	virtual void getResponse();
	virtual void assembleStiffnessMatrix(mat &K);

	vec::fixed<4> u;

	double ue, f;

	double k;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K;
};

