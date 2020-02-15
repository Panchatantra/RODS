#pragma once
#include "Basis.h"
#include "node.h"
#include <armadillo>

using namespace arma;

namespace ELE {
	enum localAxis
	{
		U1, U2, U3
	};
};

class element : public Basis
{
public:
	element();
	element(const int id);
	~element();

	virtual void buildMatrix() = 0;
	virtual void getResponse(const bool update = false) = 0;
	virtual void assembleInitialStiffnessMatrix(mat &K0) = 0;
	virtual void assembleStiffnessMatrix(mat &K) = 0;
	virtual void assembleDampingMatrix(mat &C) = 0;
	virtual void assembleMassMatrix(mat &M) = 0;
	virtual void assembleNonlinearForceVector(vec &q) = 0;

	static double dt;
	int nv;
	double *force, *deformation;
};