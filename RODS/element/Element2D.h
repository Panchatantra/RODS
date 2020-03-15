#pragma once
#include "Element.h"
#include "Node.h"

/**
 * @brief      The base class of element in X-Z plane.
 */
class Element2D :
	public Element
{
public:
	Element2D(const int id, Node * nodeI, Node * nodeJ);
	~Element2D();

	virtual void buildMatrix() {}
	virtual void getResponse(const bool update = false) {}
	virtual void assembleInitialStiffnessMatrix(mat &K0) {}
	virtual void assembleStiffnessMatrix(mat &K) {}
	virtual void assembleDampingMatrix(mat &C) {}
	virtual void assembleMassMatrix(mat &M) {}
	virtual void assembleNonlinearForceVector(vec &q) {}

	Node *nodeI, *nodeJ;

	double L;
	double lxx, lxz, lzx, lzz;
};