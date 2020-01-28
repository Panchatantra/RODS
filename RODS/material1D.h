#pragma once

#include "material.h"

class material1D :
	public material
{
public:
	material1D(const int id, const double E);
	~material1D();

	double sigma, epsilon;
};

