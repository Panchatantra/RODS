#pragma once

#include "DOF.h"
#include "Element1D.h"
#include <armadillo>

using namespace arma;

class dashpot : public Element1D
{
public:
	dashpot(const int id, DOF *i, DOF *j, const double c);
	~dashpot();

	void buildMatrix();
	void assembleDampingMatrix(mat & C);
	void getResponse(const bool update = false);

	double c;
	double u, f;

	mat::fixed<2, 2> C;
};

