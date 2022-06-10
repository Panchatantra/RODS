#pragma once
#include "Element.h"
#include "Node.h"

/**
 * @brief      The base class of 3D element.
 */
class ROD3D :
	public Element
{
public:
	ROD3D(const int id, Node * nodeI, Node * nodeJ);
	~ROD3D();

	virtual void buildMatrix() {}
	virtual void getResponse(const bool update = false) {}
	virtual void assembleInitialStiffnessMatrix(mat &K0) {}
	virtual void assembleStiffnessMatrix(mat &K) {}
	virtual void assembleDampingMatrix(mat &C) {}
	virtual void assembleMassMatrix(mat &M) {}
	virtual void assembleNonlinearForceVector(vec &q) {}

	Node *nodeI, *nodeJ;

	double L;
	double lxx;
	double lxy;
	double lxz;
	double lyx;
	double lyy;
	double lyz;
	double lzx;
	double lzy;
	double lzz;
};