#pragma once
#include "Element.h"


/**
 * @brief      The base class of plane element.
 */
class Plane2D :
	public Element
{
public:
	Plane2D(const int id);
	~Plane2D();

	virtual void buildMatrix() = 0;
	virtual void getResponse(const bool update = false) = 0;
	virtual void assembleInitialStiffnessMatrix(mat &K0) = 0;
	virtual void assembleStiffnessMatrix(mat &K) = 0;
	virtual void assembleDampingMatrix(mat &C) = 0;
	virtual void assembleMassMatrix(mat &M) = 0;
	virtual void assembleNonlinearForceVector(vec &q) = 0;

};