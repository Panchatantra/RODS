#pragma once
#include "section.h"
class sectionElasticBar :
	public section
{
public:
	sectionElasticBar(const int id, material *mat, const double A);
	sectionElasticBar(const int id, material *mat, const double A, const double Ix, const double Iy);
	~sectionElasticBar();

	double Ix, Iy;
};

