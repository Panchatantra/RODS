#pragma once
#include "element.h"
#include "node.h"

class Plane2D :
	public element
{
public:
	Plane2D(const int id, node * nodeI, node * nodeJ, node *nodeP, node *nodeQ);
	~Plane2D();

	virtual void buildMatrix() {}
	virtual void getResponse(const bool update = false) {}
	virtual void assembleInitialStiffnessMatrix(mat &K0) {}
	virtual void assembleStiffnessMatrix(mat &K) {}
	virtual void assembleDampingMatrix(mat &C) {}
	virtual void assembleMassMatrix(mat &M) {}
	virtual void assembleNonlinearForceVector(vec &q) {}

	node *nodeI, *nodeJ, *nodeP, *nodeQ;
	mat::fixed<2, 2> J;
	mat::fixed<3, 3> D;
};