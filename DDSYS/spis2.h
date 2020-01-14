#pragma once

#include "dof.h"
#include "element1D.h"
#include <armadillo>

using namespace arma;

class spis2 : public element1D
{
public:
	spis2(const int n, dof *i, dof *j, dof *in, const double m, const double c, const double k);
	~spis2();

	void buildMatrix();
	void assembleMassMatrix(mat & M);
	void assembleStiffnessMatrix(mat & K);
	void assembleDampingMatrix(mat & C);
	void getResponse();

	dof *dofIN;
	double m;
	double c;
	double k;

	mat M, C, K;

	double u, u_in, f, f_c, f_m;
};

