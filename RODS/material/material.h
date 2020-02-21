#pragma once
#include "Basis.h"

class Material : public Basis
{
public:
	Material();
	Material(const int id, const double E);
	~Material();

	virtual void setStrain(const double *strain) = 0;
	virtual void getResponse(const bool update = false) = 0;

	virtual Material *copy() = 0;

	double E;
};