#pragma once

#include <vector>
#include "ROD2D.h"
#include "section/SectionFrame2D.h"

using namespace std;

/**
 * @brief      The nonlinear frame2D element including P-Δ effect.
 */
class FramePDelta2D :
	public ROD2D
{
public:
	FramePDelta2D(const int id, Node *nodeI, Node *nodeJ, SectionFrame2D *sec, const int nIntPoints=5);
	~FramePDelta2D();

	void buildMatrix() override;
	void getResponse(const bool update = false) override;
	void assembleStiffnessMatrix(mat &K) override;
	void assembleInitialStiffnessMatrix(mat &K0) override;
	void assembleNonlinearForceVector(vec &q) override;

	vector<SectionFrame2D *> sections;
	double *xi, *wt;
	mat::fixed<3, 3> k, k0;
	vec::fixed<3> ue, f;

	mat::fixed<2,3> B;

	mat::fixed<3, 6> T;
	mat::fixed<6, 6> K0;
	mat::fixed<6, 6> K;
	mat::fixed<6, 6> Kg;
	mat::fixed<6, 6> Tg;
	vec::fixed<6> u;
	vec::fixed<6> q;
};