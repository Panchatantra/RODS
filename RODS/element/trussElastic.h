#pragma once

#include "Element2D.h"

class TrussElastic :
	public Element2D
{
public:
	TrussElastic(const int id, Node * nodeI, Node * nodeJ, const double EA);
	~TrussElastic();

	void buildMatrix();
	void getResponse(const bool update = false);
	void assembleStiffnessMatrix(mat &K);

	vec::fixed<4> u;

	double EA, k;
	double ue, f;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K;
};