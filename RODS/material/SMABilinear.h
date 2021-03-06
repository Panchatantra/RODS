#pragma once
#include "Material1D.h"

namespace MAT_SMABL {
	enum state
	{
		ELASTIC, YIELD, UNLOAD, RECOVER, RELOAD
	};
}

/**
 * @brief      The bilinear SMA material.
 */
class SMABilinear :
	public Material1D
{
public:
	SMABilinear(const int id, const double E, const double fy, const double alpha, const double sigma_shift);
	~SMABilinear();

	virtual void setStrain(const double *strain) { epsilon = *strain; }
	virtual void getResponse(const bool update = false);

	virtual Material1D *copy() { return new SMABilinear(id, E0, fy, alpha, sigma_shift); }

	double fy, alpha;
	double E0, epsilon_y, E1, sigma_shift, epsilon_shift, sigma_recover;
	double E_p, epsilon_p, sigma_p;
	double sigma_max, sigma_min;
	double epsilon_max, epsilon_min;
	double epsilon_reload;
	MAT_SMABL::state status, status_p;
};

