#include "SectionTruss.h"

SectionTruss::SectionTruss() :
	k(0.0), epsilon(0.0), f(0.0), nv(1), strain(nullptr), force(nullptr)
{
}

SectionTruss::SectionTruss(const int id, vector<Fiber *> fibers):
	Section(id), k(0.0), epsilon(0.0), f(0.0),
	nv(1), strain(nullptr), force(nullptr)
{
	this->fibers.clear();
	for (auto it=fibers.begin(); it!=fibers.end(); ++it)
	{
		this->fibers.push_back((*it)->copy());
		k += (*it)->k;
	}
}

SectionTruss::~SectionTruss()
{
}

void SectionTruss::setStrain(const double *strain)
{
	epsilon = *strain;
	Fiber *fiber = nullptr;

	for (auto it = fibers.begin(); it != fibers.end(); ++it)
	{
		fiber = *it;
		fiber->setStrain(&epsilon);
	}
}

void SectionTruss::getResponse(const bool update)
{
	f = 0.0;
	k = 0.0;
	Fiber *fiber = nullptr;

	for (auto it = fibers.begin(); it != fibers.end(); ++it)
	{
		fiber = *it;
		fiber->getResponse(update);
		f += fiber->N;
		k += fiber->k;
	}
	
	force = &f;
	strain = &epsilon;
}