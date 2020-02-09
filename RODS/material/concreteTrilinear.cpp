#include "concreteTrilinear.h"
#include <math.h>

using namespace MAT_CTL;

concreteTrilinear::concreteTrilinear(const int id, const double E, const double fc,
	const double epsilon_c, const double sigma_cr, const double sigma_u, const double epsilon_u) :
    material1D(id, E),
    fc(fc), epsilon_c(epsilon_c),
    sigma_cr(sigma_cr), epsilon_cr(sigma_cr/E),
    sigma_u(sigma_u), epsilon_u(epsilon_u),
    E0(E), E1((fc - sigma_cr)/(epsilon_c - epsilon_cr)),
    E2((sigma_u - fc)/(epsilon_u - epsilon_c)),
    epsilon_origin(0.0),
    sigma_max(sigma_cr), epsilon_max(sigma_cr/E),
    E_p(E), sigma_p(0.0), epsilon_p(0.0),
    status(ELASTIC), status_p(ELASTIC)
{
}

concreteTrilinear::~concreteTrilinear()
{
}

void concreteTrilinear::getResponse(const bool update)
{
	double depsilon = epsilon - epsilon_p;
	double sigma_try = sigma_p + depsilon * E_p;
	double epsilon_origin_ = epsilon_origin;
	double epsilon_max_ = epsilon_max;
	double sigma_max_ = sigma_max;

	status = status_p;

	switch (status)
	{
	case TENSION:
		if ((epsilon - epsilon_origin)<0)
		{
			sigma = 0.0;
			E = 0.0;
		}
		else
		{
			if (epsilon < epsilon_max)
			{
				status = UNLOAD;
				E = sigma_max / (epsilon_max - epsilon_origin);
				sigma = (epsilon - epsilon_origin)*E;
			}
			else
			{
				compressionEnvelope(epsilon, sigma, E, status);
				epsilon_max_ = epsilon;
				sigma_max_ = sigma;
			}
		}
		break;
	case ELASTIC:
		if (epsilon < 0)
		{
			status = TENSION;
			sigma = 0.0;
			E = 0.0;
		}
		else
		{
			compressionEnvelope(epsilon, sigma, E, status);
			if (epsilon > epsilon_max)
			{
				epsilon_max_ = epsilon;
				sigma_max_ = sigma;
			}
		}
		break;
	case CRACK:
		if (depsilon < 0)
		{
			status = UNLOAD;
			E = E0;
			sigma = sigma_p + E * depsilon;
			epsilon_origin_ = epsilon_p - sigma_p/E;
			if (sigma < 0)
			{
				status = TENSION;
				sigma = 0;
				E = 0.0;
			}
		}
		else
		{
			compressionEnvelope(epsilon, sigma, E, status);
			epsilon_max_ = epsilon;
			sigma_max_ = sigma;
		}
		break;
	case DESCEND:
		if (depsilon < 0)
		{
			status = UNLOAD;
			E = E0 / sqrt(epsilon_max / epsilon_c);
			sigma = sigma_p + E * depsilon;
			epsilon_origin_ = epsilon_p - sigma_p / E;

			if (sigma < 0)
			{
				status = TENSION;
				sigma = 0;
				E = 0.0;
			}
		}
		else
		{
			compressionEnvelope(epsilon, sigma, E, status);
			epsilon_max_ = epsilon;
			sigma_max_ = sigma;
		}
		break;
	case CRUSH:
		if (depsilon < 0)
		{
			status = UNLOAD;
			E = E0 / sqrt(epsilon_max / epsilon_c);
			sigma = sigma_p + E * depsilon;
			epsilon_origin_ = epsilon - sigma_p / E;

			if (sigma < 0)
			{
				status = TENSION;
				sigma = 0;
				E = 0.0;
			}
		}
		else
		{
			compressionEnvelope(epsilon, sigma, E, status);
			epsilon_max_ = epsilon;
			sigma_max_ = sigma;
		}
		break;
	case UNLOAD:
		if (sigma_try < 0)
		{
			status = TENSION;
			sigma = 0.0;
			E = 0.0;
		}
		else if (epsilon > epsilon_max)
		{
			compressionEnvelope(epsilon, sigma, E, status);
			epsilon_max_ = epsilon;
			sigma_max_ = sigma;
		}
		else
		{
			sigma = sigma_try;
			E = E_p;
		}
		break;
	default:
		break;
	}

	if (update)
	{
		epsilon_p = epsilon;
		sigma_p = sigma;
		E_p = E;
		status_p = status;
		epsilon_origin = epsilon_origin_;
		sigma_max = sigma_max_;
		epsilon_max = epsilon_max_;
	}
}

void concreteTrilinear::compressionEnvelope(const double strain, double & stress, double & tangent, MAT_CTL::state &s)
{
	if (strain < epsilon_cr)
	{
		s = ELASTIC;
		tangent = E0;
		stress = tangent * strain;
	}
	else if (strain < epsilon_c)
	{
		s = CRACK;
		tangent = E1;
		stress = sigma_cr + (strain - epsilon_cr)*tangent;
	}
	else if (strain < epsilon_u)
	{
		s = DESCEND;
		tangent = E2;
		stress = fc + (strain - epsilon_c)*tangent;
	}
	else
	{
		s = CRUSH;
		tangent = 0.0;
		stress = sigma_u;
	}
}
