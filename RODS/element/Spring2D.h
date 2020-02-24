#pragma once

#include "Element2D.h"
#include "Node.h"

/**
 * @brief      The spring in X-Z plane.
 */
class Spring2D :
	public Element2D
{
public:
	Spring2D(const int id, Node * nodeI, Node * nodeJ, const double k, RODS::LocalAxis axis);
	~Spring2D();

	virtual void buildMatrix();
	virtual void getResponse();
	virtual void assembleStiffnessMatrix(mat &K);

	vec::fixed<4> u;

	double k;
	double ue, f;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K;
};

