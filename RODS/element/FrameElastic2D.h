#pragma once

#include "element2D.h"

class FrameElastic2D :
	public element2D
{
public:
	FrameElastic2D(const int id, node * nodeI, node * nodeJ, const double EA, const double EI);
	~FrameElastic2D();

	void buildMatrix();
	void getResponse(const bool update = false);
	void assembleStiffnessMatrix(mat &K);

	double EA, EI;
	double *ue, *f;

	mat::fixed<3, 3> k;
	mat::fixed<3, 6> T;
	mat::fixed<6, 6> K;
	vec::fixed<6> u;
};