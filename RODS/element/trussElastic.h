#pragma once

#include "element2D.h"

class trussElastic :
	public element2D
{
public:
	trussElastic(const int id, node * nodeI, node * nodeJ, const double EA);
	~trussElastic();

	void buildMatrix();
	void getResponse(const bool update = false);
	void assembleStiffnessMatrix(mat &K);

	vec::fixed<4> u;

	double EA, k;
	double ue, f;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K;
};