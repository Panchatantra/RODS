#pragma once

#include "element.h"
#include "dof.h"

class element1D :
	public element
{
public:
	element1D();
	element1D(const int id, dof *i, dof *j);
	~element1D();

	virtual void buildMatrix() {}
	virtual void getResponse(const bool update = false) {}
	virtual void assembleStiffnessMatrix(mat &K) {}
	virtual void assembleDampingMatrix(mat &C) {}
	virtual void assembleMassMatrix(mat &M) {}
	virtual void assembleNonlinearForceVector(vec &q) {}

	dof *dofI, *dofJ;
};