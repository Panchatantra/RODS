#pragma once

#include "Element2D.h"

/**
 * @brief      The elastic beam element in X-Z plane.
 * @note       This element does not have an axial deformation, and thus can not be used in practice.
 */
class BeamElastic2D :
	public Element2D
{
public:
	BeamElastic2D(const int id, Node * nodeI, Node * nodeJ, const double EI);
	~BeamElastic2D();

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