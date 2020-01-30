#pragma once
#include "material1D.h"

enum elastoplasticState
{
	ELASTIC, YIELD
};

class elastoplastic :
	public material1D
{
public:
	elastoplastic(const int id, const double E, const double fy, const double alpha);
	~elastoplastic();

	virtual void setStrain(const double *strain) { epsilon = *strain; }
	virtual void getResponse(const bool update = false);

	virtual material *copy() { return new elastoplastic(id, E, fy, alpha); }

	double fy, alpha;
	double E0, epsilon_y, E1;
	double E_p, epsilon_p, depsilon_p, sigma_p;
	elastoplasticState status, status_p;
};

