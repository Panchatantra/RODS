#pragma once

#include "element2D.h"
#include "node.h"
#include "section.h"

class truss :
	public element2D
{
public:
	truss(const int id, node * nodeI, node * nodeJ, section *sec);
	~truss();

	void buildMatrix();
	void getResponse(const bool update = false);

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