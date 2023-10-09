#pragma once
#include "Element.h"
#include "Node.h"

/**
 * @brief      The base class of element in X-Z plane.
 */
class ROD2D :
	public Element
{
public:
	ROD2D();
	ROD2D(const int id, Node * nodeI, Node * nodeJ);
	ROD2D(const int id, Node * nodeI, Node * nodeJ, RODS::LocalAxis axis);
	~ROD2D();

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
	int IdNodeI, IdNodeJ;
	RODS::LocalAxis axis;
};