#pragma once

#include "Shell3D.h"
#include "Node.h"

/**
 * @brief      The 4-node rectangle elastic shell element.
 */
class RectShell4Elastic :
	public Shell3D
{
public:
	RectShell4Elastic(const int id, Node *nodeI, Node *nodeJ, Node *nodeP, Node *nodeQ,
		const double E, const double nu, const double t);
	~RectShell4Elastic();

	void buildMatrix();
	void getResponse(const bool update = false);
	void assembleStiffnessMatrix(mat &K);

	virtual void assembleInitialStiffnessMatrix(mat &K0) {}
	virtual void assembleDampingMatrix(mat &C) {}
	virtual void assembleMassMatrix(mat &M) {}
	virtual void assembleNonlinearForceVector(vec &q) {}

	double E, nu, t;
	double a, b;

	double lxx;
	double lxy;
	double lxz;
	double lyx;
	double lyy;
	double lyz;
	double lzx;
	double lzy;
	double lzz;

	mat::fixed<24, 24> K, k;
	mat::fixed<24, 24> T;
	vec::fixed<24> u, f;
};