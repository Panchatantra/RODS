#pragma once

#include <vector>
#include "section/Section.h"
#include "Fiber.h"

#include <armadillo>

using namespace std;
using namespace arma;

class SectionFrame2D : public Section
{
public:
	SectionFrame2D();
	SectionFrame2D(const int id, vector<Fiber *> fibers);
	~SectionFrame2D();

	void setStrain(const double *strain) override;
	void getResponse(const bool update = false) override;

	SectionFrame2D *copy() override {return new SectionFrame2D(id, fibers);}

	void getSectionCentroid();
	void getInitialStiffness();

	vector<Fiber *> fibers;
	mat::fixed<2,2> k, k0;
	vec::fixed<2> epsilon, f;
	double A, yc;
	int nv;
	double *strain, *force;
};