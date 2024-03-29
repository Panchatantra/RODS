#pragma once
#include "Plane2D.h"
#include "Node.h"

/**
 * @brief      The basic class of plane element.
 */
class Tri2D :
	public Plane2D
{
public:
	Tri2D(const int id, Node * nodeI, Node * nodeJ, Node *nodeP);
	~Tri2D();

	virtual void buildMatrix() {}
	virtual void getResponse(const bool update = false) {}
	virtual void assembleInitialStiffnessMatrix(mat &K0) {}
	virtual void assembleStiffnessMatrix(mat &K) {}
	virtual void assembleDampingMatrix(mat &C) {}
	virtual void assembleMassMatrix(mat &M) {}
	virtual void assembleNonlinearForceVector(vec &q) {}

	Node *nodeI, *nodeJ, *nodeP;
	mat::fixed<2, 2> J;
	mat::fixed<3, 3> D;
};