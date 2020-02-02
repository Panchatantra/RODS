#pragma once

#include "basis.h"
#include "material.h"

class section : public basis
{
public:
	section();
	section(const int id, material *mat, const double A);
	~section();

	virtual void setStrain(const double *strain) = 0;
	virtual void getResponse(const bool update = false) = 0;

	int nv;

	material *mat;
	double A;
	double *epsilon, *force;
};