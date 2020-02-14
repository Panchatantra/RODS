#pragma once

#include "Plane2D.h"

class Quad4Elastic :
	public Plane2D
{
public:
	Quad4Elastic(const int id, node *nodeI, node *nodeJ, node *nodeP, node *nodeQ,
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