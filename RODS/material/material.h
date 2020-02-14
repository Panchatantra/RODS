#pragma once
#include "Basis.h"

class material : public Basis
{
public:
	material();
	material(const int id, const double E);
	~material();

	virtual void setStrain(const double *strain) = 0;
	virtual void getResponse(const bool update = false) = 0;

	virtual material *copy() = 0;

	double E;
};