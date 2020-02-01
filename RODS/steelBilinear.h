#pragma once
#include "material1D.h"

namespace MAT_STBL {
	enum state
	{
		ELASTIC, YIELD_P, YIELD_M, UNLOAD_P, UNLOAD_M, RECOVER_P, RECOVER_M
	};
}

class steelBilinear :
	public material1D
{
public:
	steelBilinear(const int id, const double E, const double fy, const double alpha, const double beta = 0.5);
	~steelBilinear();

	virtual void setStrain(const double *strain) { epsilon = *strain; }
	virtual void getResponse(const bool update = false);

	virtual material1D *copy() { return new steelBilinear(id, E0, fy, alpha, sigma_ref/fy); }

	double fy, alpha, sigma_ref;
	double E0, epsilon_y, E1;
	double E_p, epsilon_p, sigma_p;
	double sigma_max, sigma_min;
	double epsilon_max, epsilon_min, epsilon_origin;
	MAT_STBL::state status, status_p;
};

