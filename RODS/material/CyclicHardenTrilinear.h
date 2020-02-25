#pragma once
#include "Material1D.h"

namespace MAT_CHT {
	enum State
	{
		Elastic = 1,
		Yield1P,
		Yield2P,
		UnloadP,
		RecoverP,
		Yield1M,
		Yield2M,
		UnloadM,
		RecoverM
	};
}

/**
 * @brief      The trilinear cyclic harden material.
 */
class CyclicHardenTrilinear :
	public Material1D
{
public:
	CyclicHardenTrilinear(const int id, const double E, const double sigma1, const double alpha1,
							const double sigma2, const double alpha2);
	~CyclicHardenTrilinear();

	virtual void setStrain(const double *strain) { epsilon = *strain; }
	virtual void getResponse(const bool update = false);

	virtual Material1D *copy() { return new CyclicHardenTrilinear(id, E0, sigma1, alpha1, sigma2, alpha2); }

	void changeToStateElastic();
	void changeToStateYield1P();
	void changeToStateYield2P();
	void changeToStateUnloadP();
	void changeToStateRecoverP();
	void changeToStateYield1M();
	void changeToStateYield2M();
	void changeToStateUnloadM();
	void changeToStateRecoverM();

	double sigma1;
	double alpha1;
	double sigma2;
	double alpha2;
	double E0;
	double E1;
	double E2;
	double epsilon1;
	double epsilon2;
	double sigma_max;
	double epsilon_max;
	double sigma_min;
	double epsilon_min;
	double sigma_max_;
	double epsilon_max_;
	double sigma_min_;
	double epsilon_min_;
	double E_p;
	double sigma_p;
	double epsilon_p;
	double sigma_rp;
	double epsilon_rp;
	double sigma_rn;
	double epsilon_rn;
	double epsilon_origin;
	MAT_CHT::State state, state_p;
};

