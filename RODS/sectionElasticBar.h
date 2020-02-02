#pragma once
#include "section.h"
class sectionElasticBar :
	public section
{
public:
	sectionElasticBar(const int id, material *mat, const double A);
	sectionElasticBar(const int id, material *mat, const double A, const double Ix, const double Iy);
	~sectionElasticBar();

	virtual void setStrain(const double *strain);
	virtual void getResponse(const bool update = false);

	double Ix, Iy;
	double eps, kpx, kpy;
	double N, Mx, My;
};

