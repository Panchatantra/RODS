#pragma once

#include "ROD3D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The 3D inerter.
 */
class Inerter3D : public ROD3D
{
public:
	Inerter3D(const int id, Node * nodeI, Node * nodeJ, double m, RODS::LocalAxis axis);
	~Inerter3D();

	virtual void buildMatrix();
	virtual void assembleMassMatrix(mat &M);
	virtual void getResponse(const bool update = false);

	double m;

	rowvec::fixed<6> T;
	mat::fixed<6, 6> M;

	double ue, f;
};

