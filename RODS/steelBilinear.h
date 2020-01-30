#pragma once
#include "material1D.h"

enum steelBilinearState
{
	ELASTIC, YIELD_P, YIELD_M, UNLOAD_P, UNLOAD_M, RECOVER_P, RECOVER_M
};

class steelBilinear :
	public material1D
{
public:
	steelBilinear(const int id, const double E, const double fy, const double alpha);
	~steelBilinear();

	virtual void setStrain(const double *strain) { epsilon = *strain; }
	virtual void getResponse(const bool update = false);

	virtual material *copy() { return new steelBilinear(id, E, fy, alpha); }

	double fy, alpha;
	double E0, epsilon_y, E1;
	double E_p, epsilon_p, depsilon_p, sigma_p;
	steelBilinearState status, status_p;
};

