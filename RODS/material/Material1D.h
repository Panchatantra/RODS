#pragma once

#include "Material.h"

class Material1D :
	public Material
{
public:
	Material1D(const int id, const double E);
	~Material1D();

	virtual void setStrain(const double *strain) = 0;
	virtual void getResponse(const bool update = false) = 0;

	virtual Material1D *copy() = 0;

	double sigma, epsilon;
};

