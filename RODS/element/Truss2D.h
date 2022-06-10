#pragma once

#include "ROD2D.h"
#include "section/SectionTruss.h"

/**
 * @brief      The nonlinear truss element in X-Z plane.
 */
class Truss2D :
	public ROD2D
{
public:
	Truss2D(const int id, Node *nodeI, Node *nodeJ, SectionTruss *sec);
	~Truss2D();

	void buildMatrix() override;
	void getResponse(const bool update = false) override;
	void assembleInitialStiffnessMatrix(mat &K0) override;
	void assembleStiffnessMatrix(mat &K) override;
	void assembleNonlinearForceVector(vec &q) override;

	SectionTruss* sec;

	double k, k0;
	double ue, f;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K0;
	mat::fixed<4, 4> K;
	vec::fixed<4> u;
	vec::fixed<4> q;
};