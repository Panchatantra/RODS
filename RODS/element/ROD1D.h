#pragma once

#include "Element.h"
#include "DOF.h"

/**
 * @brief      The base class of element in X direction.
 */
class ROD1D :
	public Element
{
public:
	ROD1D();
	ROD1D(const int id, DOF *i, DOF *j);
	~ROD1D();

	virtual void buildMatrix() {}
	virtual void getResponse(const bool update = false) {}
	virtual void assembleInitialStiffnessMatrix(mat &K0) {}
	virtual void assembleStiffnessMatrix(mat &K) {}
	virtual void assembleDampingMatrix(mat &C) {}
	virtual void assembleMassMatrix(mat &M) {}
	virtual void assembleNonlinearForceVector(vec &q) {}

	DOF *dofI, *dofJ;
};