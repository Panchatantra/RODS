#pragma once

#include "Basis.h"

/**
 * @brief      The basic class of nonlinear section.
 */
class Section : public Basis
{
public:
	Section();
	Section(const int id);
	~Section();

	virtual void setStrain(const double *strain) = 0;
	virtual void getResponse(const bool update = false) = 0;

	virtual Section *copy() = 0;

	int nv;

	double A;
	double *strain, *force;
};