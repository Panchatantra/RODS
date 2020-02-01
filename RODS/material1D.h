#pragma once

#include "material.h"

class material1D :
	public material
{
public:
	material1D(const int id, const double E);
	~material1D();

	virtual void setStrain(const double *strain) = 0;
	virtual void getResponse(const bool update = false) = 0;

	virtual material1D *copy() = 0;

	double sigma, epsilon;
};

