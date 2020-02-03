#pragma once
#include "element.h"
#include "node.h"

class element2D :
	public element
{
public:
	element2D(const int id, node * nodeI, node * nodeJ);
	~element2D();

	virtual void buildMatrix() {}
	virtual void getResponse(const bool update = false) {}
	virtual void assembleStiffnessMatrix(mat &K) {}
	virtual void assembleDampingMatrix(mat &C) {}
	virtual void assembleMassMatrix(mat &M) {}
	virtual void assembleNonlinearForceVector(vec &q) {}

	node *nodeI, *nodeJ;
};