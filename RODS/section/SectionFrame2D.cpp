#include "SectionFrame2D.h"

SectionFrame2D::SectionFrame2D() :
	A(0.0), yc(0.0),
	nv(3), strain(nullptr), force(nullptr)
{
}

SectionFrame2D::SectionFrame2D(const int id, vector<Fiber *> fibers):
	Section(id), A(0.0), yc(0.0),
	nv(3), strain(nullptr), force(nullptr)
{
	this->fibers.clear();
	for (auto it=fibers.begin(); it!=fibers.end(); ++it)
	{
		this->fibers.push_back((*it)->copy());
	}

	void getSectionCentroid();
	void getInitialStiffness();
}

SectionFrame2D::~SectionFrame2D()
{
}

void SectionFrame2D::setStrain(const double *strain)
{
	epsilon(0) = strain[0];
	epsilon(1) = strain[1];
}

void SectionFrame2D::getResponse(const bool update)
{
	k.fill(0.0);
	f.fill(0.0);
	Fiber *fiber = nullptr;

	double y = 0.0;
	double strainFiber = 0.0;

	for (auto it = fibers.begin(); it != fibers.end(); ++it)
	{
		fiber = *it;
		y = fiber->y - yc;
		strainFiber = epsilon(0) - y*epsilon(1);
		fiber->setStrain(&strainFiber);
		fiber->getResponse(update);

		f(0) += fiber->N;
		f(1) += -fiber->N*y;

		k(0,0) += fiber->k;
		k(0,1) += -fiber->k*y;
		k(1,1) += fiber->k*y*y;
	}

	k(1,0) = k(0,1);

	force = f.memptr();
	strain = epsilon.memptr();
}

void SectionFrame2D::getSectionCentroid()
{
	Fiber *fiber = nullptr;

	A = 0.0;
	yc = 0.0;
	for (auto it = fibers.begin(); it != fibers.end(); ++it)
	{
		fiber = *it;
		A += fiber->A;
		yc += fiber->A*fiber->y;
	}
	yc = yc/A;
}

void SectionFrame2D::getInitialStiffness()
{
	k0.fill(0.0);
	Fiber *fiber = nullptr;
	double y = 0.0;
	double strainFiber = 0.0;

	for (auto it = fibers.begin(); it != fibers.end(); ++it)
	{
		fiber = *it;
		y = fiber->y - yc;
		strainFiber = epsilon(0) - y*epsilon(1);
		fiber->setStrain(&strainFiber);
		fiber->getResponse();

		k0(0,0) += fiber->k;
		k0(0,1) += -fiber->k*y;
		k0(1,1) += fiber->k*y*y;
	}

	k0(1,0) = k0(0,1);
	k = k0;
}
