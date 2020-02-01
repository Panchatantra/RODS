#pragma once
#include "material1D.h"

namespace MAT_CTL {
	enum state
	{
		TENSION, ELASTIC, CRACK, DESCEND, CRUSH, UNLOAD
	};
}

class concreteTrilinear :
	public material1D
{
public:
	concreteTrilinear(const int id, const double E, const double fc, const double epsilon_c, 
		const double sigma_cr, const double sigma_u, const double epsilon_u);
	~concreteTrilinear();

	virtual void setStrain(const double *strain) { epsilon = *strain; }
	virtual void getResponse(const bool update = false);

	virtual material1D *copy() { return new concreteTrilinear(id, E0, fc, epsilon_c, sigma_cr, sigma_u, epsilon_u); }

	void compressionEnvelope(const double strain, double &stress, double &tangent, MAT_CTL::state &s);


	double sigma_cr, fc, sigma_u;
	double epsilon_c, epsilon_u;
	double epsilon_origin, epsilon_max, sigma_max;
	double E0, epsilon_cr, E1, E2;
	double E_p, epsilon_p, sigma_p;
	MAT_CTL::state status, status_p;
};

