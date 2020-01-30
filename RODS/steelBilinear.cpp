#include "steelBilinear.h"

steelBilinear::steelBilinear(const int id, const double E, const double fy, const double alpha) :
	material1D(id, E), fy(fy), alpha(alpha), E0(E), epsilon_y(fy / E), E1(E*alpha),
	status(ELASTIC), status_p(ELASTIC)
{
}

steelBilinear::~steelBilinear()
{
}

void steelBilinear::getResponse(const bool update)
{
	double depslion = epsilon - epsilon_p;
	double sigma_try = sigma_p + depslion * E_p;

	status = status_p;

	switch (status)
	{
	case ELASTIC:
		if (sigma_try >= fy)
		{
			sigma = fy + (epsilon - epsilon_y)*E1;
			E = E1;
			status = YIELD_P;
		}
		else if (sigma_try <= -fy)
		{
			sigma = -fy + (epsilon - epsilon_y)*E1;
			E = E1;
			status = YIELD_M;
		}
		break;
	case YIELD_P:
		break;
	case YIELD_M:
		break;
	case UNLOAD_P:
		break;
	case UNLOAD_M:
		break;
	case RECOVER_P:
		break;
	case RECOVER_M:
		break;
	default:
		break;
	}

	if (update)
	{
		epsilon_p = epsilon;
		depsilon_p = depslion;
		sigma_p = sigma;
		E_p = E;
		status_p = status;
	}
}