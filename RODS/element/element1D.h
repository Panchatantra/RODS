#pragma once

#include "element.h"
#include "DOF.h"

class element1D :
	public element
{
public:
	element1D();
	element1D(const int id, DOF *i, DOF *j);
	~element1D();

	virtual void buildMatrix() {}
	virtual void getResponse(const bool update = false) {}
	virtual void assembleInitialStiffnessMatrix(mat &K0) {}
	virtual void assembleStiffnessMatrix(mat &K) {}
	virtual void assembleDampingMatrix(mat &C) {}
	virtual void assembleMassMatrix(mat &M) {}
	virtual void assembleNonlinearForceVector(vec &q) {}

	DOF *dofI, *dofJ;
};