#pragma once

#include "element2D.h"
#include "node.h"
#include "section.h"

class truss2D :
	public element2D
{
public:
	truss2D(const int id, node * nodeI, node * nodeJ, section *sec);
	~truss2D();

	void buildMatrix();
	void getResponse();

	node * nodeI, * nodeJ;
	section * sec;

	vec::fixed<4> u;

	double ue, f;

	double L;
	double lxx, lxy;
	double k;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K;
};