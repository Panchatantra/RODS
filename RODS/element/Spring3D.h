#pragma once

#include "ROD3D.h"

/**
 * @brief      The 3D spring.
 */
class Spring3D :
	public ROD3D
{
public:
	Spring3D(const int id, Node * nodeI, Node * nodeJ, const double k, RODS::LocalAxis axis);
	~Spring3D();

	virtual void buildMatrix();
	virtual void getResponse();
	virtual void assembleStiffnessMatrix(mat &K);

	double k;
	double ue, f;

	rowvec::fixed<6> T;
	mat::fixed<6, 6> K;
};

