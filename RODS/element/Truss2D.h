#pragma once

#include "element2D.h"
#include "section/SectionTruss.h"

class Truss2D :
	public element2D
{
public:
	Truss2D(const int id, node *nodeI, node *nodeJ, SectionTruss *sec);
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