#pragma once
#include "Material1D.h"

class Elastic :
	public Material1D
{
public:
	Elastic(const int id, const double E);
	~Elastic();

	virtual void setStrain(const double *strain) { epsilon = *strain; }
	virtual void getResponse(const bool update = false) { sigma = E * epsilon; }

	virtual Material1D *copy() { return new Elastic(id, E); }
};

