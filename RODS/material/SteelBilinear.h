#pragma once
#include "Material1D.h"

namespace MAT_STBL {
	enum state
	{
		ELASTIC, YIELD_P, YIELD_M, UNLOAD_P, UNLOAD_M, RECOVER_P, RECOVER_M
	};
}

/**
 * @brief      The bilinear steel material.
 */
class SteelBilinear :
	public Material1D
{
public:
	SteelBilinear(const int id, const double E, const double fy, const double alpha, const double beta = 0.5);
	~SteelBilinear();

	virtual void setStrain(const double *strain) { epsilon = *strain; }
	virtual void getResponse(const bool update = false);

	virtual Material1D *copy() { return new SteelBilinear(id, E0, fy, alpha, sigma_ref/fy); }

	double fy, alpha, sigma_ref;
	double E0, epsilon_y, E1;
	double E_p, epsilon_p, sigma_p;
	double sigma_max, sigma_min;
	double epsilon_max, epsilon_min, epsilon_origin;
	MAT_STBL::state status, status_p;
};

