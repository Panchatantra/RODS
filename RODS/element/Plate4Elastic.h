#pragma once

#include "Quad2D.h"

/**
 * @brief      The 4-node rectangle elastic plane element.
 */
class Plate4Elastic :
	public Quad2D
{
public:
	Plate4Elastic(const int id, Node *nodeI, Node *nodeJ, Node *nodeP, Node *nodeQ,
		const double E, const double nu, const double t);
	~Plate4Elastic();

	void buildMatrix();
	void getResponse(const bool update = false);
	void assembleStiffnessMatrix(mat &K);

	double E, nu, t;
	double a, b;

	mat::fixed<12, 12> K;
	vec::fixed<12> u, f;
};