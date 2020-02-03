#pragma once

#include "element2D.h"
#include "node.h"

class spring2D :
	public element2D
{
public:
	spring2D(const int id, node * nodeI, node * nodeJ, const double k, 
		ELE::localAxis un);
	~spring2D();

	void buildMatrix();
	void getResponse();

	node *nodeI, *nodeJ;

	vec::fixed<4> u;

	double ue, f;

	double k;
	double L;
	double lxx, lxy, lyx, lyy;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K;
};

