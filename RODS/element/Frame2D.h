#pragma once

#include <vector>
#include "element2D.h"
#include "section/SectionFrame2D.h"

using namespace std;

class Frame2D :
	public element2D
{
public:
	Frame2D(const int id, node *nodeI, node *nodeJ, SectionFrame2D *sec, const int nIntPoints);
	~Frame2D();

	void buildMatrix();
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
	vec::fixed<6> u;
	vec::fixed<6> q;
};