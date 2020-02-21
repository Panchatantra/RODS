#pragma once
#include "Element.h"
#include "Node.h"

class Plane2D :
	public Element
{
public:
	Plane2D(const int id, Node * nodeI, Node * nodeJ, Node *nodeP, Node *nodeQ);
	~Plane2D();

	virtual void buildMatrix() {}
	virtual void getResponse(const bool update = false) {}
	virtual void assembleInitialStiffnessMatrix(mat &K0) {}
	virtual void assembleStiffnessMatrix(mat &K) {}
	virtual void assembleDampingMatrix(mat &C) {}
	virtual void assembleMassMatrix(mat &M) {}
	virtual void assembleNonlinearForceVector(vec &q) {}

	Node *nodeI, *nodeJ, *nodeP, *nodeQ;
	mat::fixed<2, 2> J;
	mat::fixed<3, 3> D;
};