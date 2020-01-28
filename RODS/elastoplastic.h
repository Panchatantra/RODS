#pragma once
#include "material1D.h"
class elastoplastic :
	public material1D
{
public:
	elastoplastic(const int id, const double E, const double fy, const double alpha);
	~elastoplastic();

	virtual void setStrain(const double *strain) { epsilon = *strain; }
	virtual void getResponse(const bool update = false) { sigma = E * epsilon; }

	virtual material *copy() { return new elastoplastic(id, E, fy, alpha); }

	double fy, alpha;
	double E_t, epsilon_y, E1;
	double E_p, epsilon_p, depsilon_p, sigma_p;
	size_t status, status_p;
};

