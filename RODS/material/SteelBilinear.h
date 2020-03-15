#pragma once
#include "Material1D.h"

namespace MAT_STBL {
	enum State
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

	void changeToStateElastic();
	void changeToStateYieldP();
	void changeToStateUnloadP();
	void changeToStateRecoverP();
	void changeToStateYieldM();
	void changeToStateUnloadM();
	void changeToStateRecoverM();

	virtual Material1D *copy() { return new SteelBilinear(id, E0, fy, alpha, sigma_ref/fy); }

	double fy, alpha, sigma_ref;
	double E0, epsilon_y, E1;
	double E_p, epsilon_p, sigma_p;
	double sigma_max, sigma_min;
	double epsilon_max, epsilon_min, epsilon_origin;
	double epsilon_unload, sigma_unload;
	double epsilon_recover, sigma_recover;
	double sigma_max_, sigma_min_;
	double epsilon_max_, epsilon_min_;
	double epsilon_unload_, sigma_unload_;
	
	MAT_STBL::State state, state_p;
};

