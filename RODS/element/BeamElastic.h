#pragma once

#include "Element2D.h"

class BeamElastic :
	public Element2D
{
public:
	BeamElastic(const int id, Node * nodeI, Node * nodeJ, const double EI);
	~BeamElastic();

	void buildMatrix();
	void getResponse(const bool update = false);
	void assembleStiffnessMatrix(mat &K);


	double EI;
	double *ue, *f;

	mat::fixed<2, 2> k;
	mat::fixed<2, 6> T;
	mat::fixed<6, 6> K;
	vec::fixed<6> u;
};