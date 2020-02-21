#pragma once

#include "DOF.h"
#include "Element1D.h"
#include <armadillo>

using namespace arma;

class Slider : public Element1D
{
public:
	Slider(const int id, DOF *i, DOF *j, const double muN);
	~Slider();

	void getResponse(const bool update=false);
	void assembleNonlinearForceVector(vec &q);

	double muN;
	double u, f, v;

	vec::fixed<2> q;
};

