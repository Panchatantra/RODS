#pragma once

#include "ROD3D.h"

/**
 * @brief      The elastic 3D truss element.
 */
class TrussElastic3D :
	public ROD3D
{
public:
	TrussElastic3D(const int id, Node * nodeI, Node * nodeJ, const double EA);
	~TrussElastic3D();

	void buildMatrix();
	void getResponse(const bool update = false);
	void assembleStiffnessMatrix(mat &K);

	double EA, k;
	double ue, f;

	rowvec::fixed<6> T;
	mat::fixed<6, 6> K;
};