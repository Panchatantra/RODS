#pragma once

#include "Quad2D.h"

/**
 * @brief      The 4-node elastic plane element.
 */
class Quad4Elastic :
	public Quad2D
{
public:
	Quad4Elastic(const int id, Node *nodeI, Node *nodeJ, Node *nodeP, Node *nodeQ,
		const double E, const double nu, const double t);
	~Quad4Elastic();

	void buildMatrix();
	void getResponse(const bool update = false);
	void assembleStiffnessMatrix(mat &K);

	void calcShapeFunction(const double xi, const double eta);

	double E, nu, t;
	double detJ;

	mat::fixed<1,4> N;
	mat::fixed<2,4> dN;
	mat::fixed<3,8> B;
	mat::fixed<8, 8> K;
	vec::fixed<8> u, f;
};