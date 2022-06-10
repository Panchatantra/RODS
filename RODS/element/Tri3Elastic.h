#pragma once

#include "Tri2D.h"

/**
 * @brief      The 3-node elastic plane element.
 */
class Tri3Elastic :
	public Tri2D
{
public:
	Tri3Elastic(const int id, Node *nodeI, Node *nodeJ, Node *nodeP,
		const double E, const double nu, const double t);
	~Tri3Elastic();

	void buildMatrix();
	void getResponse(const bool update = false);
	void assembleStiffnessMatrix(mat &K);

	double E, nu, t;
	double A;
	
	mat::fixed<3, 6> B;
	mat::fixed<6, 6> K;
	vec::fixed<6> u, f;
};