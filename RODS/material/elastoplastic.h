#pragma once
#include "Material1D.h"

namespace MAT_EP {
	enum state
	{
		ELASTIC, YIELD
	};
}

class Elastoplastic :
	public Material1D
{
public:
	Elastoplastic(const int id, const double E, const double fy, const double alpha=0.02);
	~Elastoplastic();

	virtual void setStrain(const double *strain) { epsilon = *strain; }
	virtual void getResponse(const bool update = false);

	virtual Material1D *copy() { return new Elastoplastic(id, E0, fy, alpha); }

	double fy, alpha;
	double E0, epsilon_y, E1;
	double E_p, epsilon_p, depsilon_p, sigma_p;
	MAT_EP::state status, status_p;
};

