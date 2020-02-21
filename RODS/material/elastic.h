#pragma once
#include "Material1D.h"

class elastic :
	public Material1D
{
public:
	elastic(const int id, const double E);
	~elastic();

	virtual void setStrain(const double *strain) { epsilon = *strain; }
	virtual void getResponse(const bool update = false) { sigma = E * epsilon; }

	virtual Material1D *copy() { return new elastic(id, E); }
};

