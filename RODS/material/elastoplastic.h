#pragma once
#include "material1D.h"

namespace MAT_EP {
	enum state
	{
		ELASTIC, YIELD
	};
}


class elastoplastic :
	public material1D
{
public:
	elastoplastic(const int id, const double E, const double fy, const double alpha=0.02);
	~elastoplastic();

	virtual void setStrain(const double *strain) { epsilon = *strain; }
	virtual void getResponse(const bool update = false);

	virtual material1D *copy() { return new elastoplastic(id, E0, fy, alpha); }

	double fy, alpha;
	double E0, epsilon_y, E1;
	double E_p, epsilon_p, depsilon_p, sigma_p;
	MAT_EP::state status, status_p;
};

