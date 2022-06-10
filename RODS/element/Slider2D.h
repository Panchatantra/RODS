#pragma once

#include "ROD2D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The Bouc-Wen spring in X-Z plane.
 */
class Slider2D : public ROD2D
{
public:
	Slider2D(const int id, Node * nodeI, Node * nodeJ, const double mu, const double ka);
	~Slider2D();

	void buildMatrix() override;
	void getResponse(const bool update = false) override;
	void assembleStiffnessMatrix(mat &K) override;
	void assembleNonlinearForceVector(vec &q) override;

	double mu, ka;

	mat::fixed<2, 4> T;
	mat::fixed<4, 4> K;
	vec::fixed<4> q;
	vec::fixed<2> f;
	vec::fixed<2> ue;
	vec::fixed<2> ve;
};

