#include "Elastoplastic.h"

using namespace MAT_EP;

Elastoplastic::Elastoplastic(const int id, const double E, const double fy, const double alpha) :
	Material1D(id, E), fy(fy), alpha(alpha), E0(E), epsilon_y(fy / E), E1(E*alpha),
	E_p(E), epsilon_p(0.0), depsilon_p(0.0), sigma_p(0.0),
	status(ELASTIC), status_p(ELASTIC)
{
}

Elastoplastic::~Elastoplastic()
{
}

void Elastoplastic::getResponse(const bool update)
{
	double depsilon = epsilon - epsilon_p;
	double f_try = sigma_p + depsilon * E_p;
	double bup = fy + E1 * (epsilon - epsilon_y);
	double bdn = -fy + E1 * (epsilon + epsilon_y);

	status = status_p;

	if (depsilon == 0.0)
	{
		E = E_p;
		sigma = sigma_p;
	}
	else if (status == ELASTIC)
	{
		if (depsilon > 0.0)
		{
			if (f_try > bup)
			{
				sigma = bup;
				E = E1;
				status = YIELD;
			}
			else
			{
				E = E0;
				sigma = f_try;
			}
		}
		else
		{
			if (f_try < bdn)
			{
				sigma = bdn;
				E = E1;
				status = YIELD;
			}
			else
			{
				E = E0;
				sigma = f_try;
			}
		}
	}
	else if (status == YIELD)
	{
		if (depsilon_p*depsilon > 0)
		{
			sigma = f_try;
			E = E1;
		}
		else
		{
			E = E0;
			sigma = sigma_p + E * depsilon;
			status = ELASTIC;
			if (sigma > bup)
			{
				sigma = bup;
				E = E1;
				status = YIELD;
			}
			else if (sigma < bdn)
			{
				sigma = bdn;
				E = E1;
				status = YIELD;
			}
		}
	}

	if (update)
	{
		epsilon_p = epsilon;
		depsilon_p = depsilon;
		sigma_p = sigma;
		E_p = E;
		status_p = status;
	}
}