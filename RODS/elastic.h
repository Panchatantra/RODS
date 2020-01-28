#pragma once
#include "material1D.h"

class elastic :
	public material1D
{
public:
	elastic(const int id, const double E);
	~elastic();

	virtual void setStrain(const double *strain) { epsilon = *strain; }
	virtual void getResponse(const bool update = false) { sigma = E * epsilon; }

	virtual material *copy() { return new elastic(id, E); }
};

