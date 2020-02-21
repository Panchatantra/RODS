#pragma once

#include "Element2D.h"

class trussElastic :
	public Element2D
{
public:
	trussElastic(const int id, Node * nodeI, Node * nodeJ, const double EA);
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