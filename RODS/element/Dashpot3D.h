#pragma once

#include "ROD3D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The linear 3D dashpot.
 */
class Dashpot3D : public ROD3D
{
public:
	Dashpot3D(const int id, Node * nodeI, Node * nodeJ, double c, RODS::LocalAxis axis);
	~Dashpot3D();

	virtual void buildMatrix();
	virtual void assembleDampingMatrix(mat &C);
	virtual void getResponse(const bool update = false);

	double c;
	double ue, f;

	rowvec::fixed<6> T;
	mat::fixed<6, 6> C;
};

