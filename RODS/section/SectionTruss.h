#pragma once

#include <vector>
#include "section/Section.h"
#include "Fiber.h"

using namespace std;

/**
 * @brief      The nonlinear truss section.
 */
class SectionTruss : public Section
{
public:
	SectionTruss();
	SectionTruss(const int id);
	SectionTruss(const int id, vector<Fiber *> fibers);
	~SectionTruss();

	void setStrain(const double *strain) override;
	void getResponse(const bool update = false) override;

	SectionTruss *copy() override {return new SectionTruss(id, fibers);};

	void addFiber(Fiber *fiber);
	void addFibers(vector<Fiber *> fibers);

	vector<Fiber *> fibers;
	double k, epsilon, f;
	int nv;
	double *strain, *force;
};