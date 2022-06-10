#pragma once
#include "Element.h"
#include "Node.h"


/**
 * @brief      The base class of plane element.
 */
class Shell3D :
	public Element
{
public:
	Shell3D(const int id, Node *nodeI, Node *nodeJ, Node *nodeP, Node *nodeQ);
	~Shell3D();

	virtual void buildMatrix() = 0;
	virtual void getResponse(const bool update = false) = 0;
	virtual void assembleInitialStiffnessMatrix(mat &K0) = 0;
	virtual void assembleStiffnessMatrix(mat &K) = 0;
	virtual void assembleDampingMatrix(mat &C) = 0;
	virtual void assembleMassMatrix(mat &M) = 0;
	virtual void assembleNonlinearForceVector(vec &q) = 0;

	Node *nodeI, *nodeJ, *nodeP, *nodeQ;
};