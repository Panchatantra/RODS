#include "steelBilinear.h"

using namespace MAT_STBL;

steelBilinear::steelBilinear(const int id, const double E, const double fy, const double alpha, const double beta) :
	material1D(id, E), fy(fy), alpha(alpha), E0(E),
	epsilon_y(fy / E), E1(E*alpha), sigma_ref(beta*fy),
	E_p(E), epsilon_p(0.0), sigma_p(0.0),
	sigma_max(fy), sigma_min(-fy),
	epsilon_max(fy / E), epsilon_min(-fy / E), epsilon_origin(0.0),
	status(ELASTIC), status_p(ELASTIC)
{
}

steelBilinear::~steelBilinear()
{
}

void steelBilinear::getResponse(const bool update)
{
	double depsilon = epsilon - epsilon_p;
	double sigma_try = sigma_p + depsilon * E_p;
	double sigma_max_ = sigma_max;
	double sigma_min_ = sigma_min;
	double epsilon_max_ = epsilon_max;
	double epsilon_min_ = epsilon_min;
	double epsilon_origin_ = epsilon_origin;
	double epsilon_recover = 0.0;
	double sigma_recover = 0.0;

	status = status_p;

	switch (status)
	{
	case ELASTIC:
		if (sigma_try >= fy)
		{
			status = YIELD_P;
			sigma = fy + (epsilon - epsilon_y)*E1;
			E = E1;
			sigma_max_ = sigma;
			epsilon_max_ = epsilon;
		}
		else if (sigma_try <= -fy)
		{
			status = YIELD_M;
			sigma = -fy + (epsilon - epsilon_y)*E1;
			E = E1;
			sigma_min_ = sigma;
			epsilon_min_ = epsilon;
		}
		else
		{
			sigma = sigma_try;
			E = E_p;
		}
		break;
	case YIELD_P:
		if (depsilon < 0)
		{
			status = UNLOAD_P;
			E = E0;
			sigma = sigma_p + E * depsilon;
			epsilon_origin_ = epsilon_p - sigma_p / E0;

			if (sigma < -sigma_ref)
			{
				status = RECOVER_P;
				sigma_recover = -sigma_ref;
				epsilon_recover = epsilon_origin_ + sigma_recover / E0;
				E = (sigma_recover - sigma_min) / (epsilon_recover - epsilon_min);
				sigma = sigma_recover + (epsilon - epsilon_recover)*E;

				if (sigma < sigma_min)
				{
					status = YIELD_M;
					sigma = -fy + (epsilon - epsilon_y)*E1;
					E = E1;
					sigma_min_ = sigma;
					epsilon_min_ = epsilon;
				}
			}
		}
		else
		{
			sigma = sigma_try;
			E = E_p;
			sigma_max_ = sigma;
			epsilon_max_ = epsilon;
		}
		break;
	case YIELD_M:
		if (depsilon > 0)
		{
			status = UNLOAD_M;
			E = E0;
			sigma = sigma_p + E * depsilon;
			epsilon_origin_ = epsilon_p - sigma_p / E0;

			if (sigma > sigma_ref)
			{
				status = RECOVER_M;
				sigma_recover = sigma_ref;
				epsilon_recover = epsilon_origin_ + sigma_recover / E0;
				E = (sigma_recover - sigma_max) / (epsilon_recover - epsilon_max);
				sigma = sigma_recover + (epsilon - epsilon_recover)*E;

				if (sigma > sigma_max)
				{
					status = YIELD_P;
					sigma = fy + (epsilon - epsilon_y)*E1;
					E = E1;
					sigma_max_ = sigma;
					epsilon_max_ = epsilon;
				}
			}
		}
		else
		{
			sigma = sigma_try;
			E = E_p;
			sigma_min_ = sigma;
			epsilon_min_ = epsilon;
		}
		break;
	case UNLOAD_P:
		if (sigma_try > sigma_max)
		{
			status = YIELD_P;
			sigma = fy + (epsilon - epsilon_y)*E1;
			E = E1;
			sigma_max_ = sigma;
			epsilon_max_ = epsilon;
		}
		else if (sigma_try < -sigma_ref)
		{
			status = RECOVER_P;
			sigma_recover = -sigma_ref;
			epsilon_recover = epsilon_origin + sigma_recover / E0;
			E = (sigma_recover - sigma_min) / (epsilon_recover - epsilon_min);
			sigma = sigma_recover + (epsilon - epsilon_recover)*E;

			if (sigma < sigma_min)
			{
				status = YIELD_M;
				sigma = -fy + (epsilon - epsilon_y)*E1;
				E = E1;
				sigma_min_ = sigma;
				epsilon_min_ = epsilon;
			}
		}
		else
		{
			sigma = sigma_try;
			E = E_p;
		}
		break;
	case UNLOAD_M:
		if (sigma_try < sigma_min)
		{
			status = YIELD_M;
			sigma = fy + (epsilon - epsilon_y)*E1;
			E = E1;
			sigma_min_ = sigma;
			epsilon_min_ = epsilon;
		}
		else if (sigma_try > sigma_ref)
		{
			status = RECOVER_M;
			sigma_recover = sigma_ref;
			epsilon_recover = epsilon_origin + sigma_recover / E0;
			E = (sigma_recover - sigma_max) / (epsilon_recover - epsilon_max);
			sigma = sigma_recover + (epsilon - epsilon_recover)*E;

			if (sigma > sigma_max)
			{
				status = YIELD_P;
				sigma = fy + (epsilon - epsilon_y)*E1;
				E = E1;
				sigma_max_ = sigma;
				epsilon_max_ = epsilon;
			}
		}
		else
		{
			sigma = sigma_try;
			E = E_p;
		}
		break;
	case RECOVER_P:
		if (depsilon > 0)
		{
			status = UNLOAD_M;
			E = E0;
			sigma = sigma_p + E * depsilon;
			epsilon_origin_ = epsilon_p - sigma_p / E0;

			if (sigma > sigma_ref)
			{
				status = RECOVER_M;
				sigma_recover = sigma_ref;
				epsilon_recover = epsilon_origin_ + sigma_recover / E0;
				E = (sigma_recover - sigma_max) / (epsilon_recover - epsilon_max);
				sigma = sigma_recover + (epsilon - epsilon_recover)*E;

				if (sigma > sigma_max)
				{
					status = YIELD_P;
					sigma = fy + (epsilon - epsilon_y)*E1;
					E = E1;
					sigma_max_ = sigma;
					epsilon_max_ = epsilon;
				}
			}
		}
		else if (sigma_try < sigma_min)
		{
			status = YIELD_M;
			sigma = -fy + (epsilon - epsilon_y)*E1;
			E = E1;
			sigma_min_ = sigma;
			epsilon_min_ = epsilon;
		}
		else
		{
			sigma = sigma_try;
			E = E_p;
		}
		break;
	case RECOVER_M:
		if (depsilon < 0)
		{
			status = UNLOAD_P;
			E = E0;
			sigma = sigma_p + E * depsilon;
			epsilon_origin_ = epsilon_p - sigma_p / E0;

			if (sigma < -sigma_ref)
			{
				status = RECOVER_P;
				sigma_recover = -sigma_ref;
				epsilon_recover = epsilon_origin_ + sigma_recover / E0;
				E = (sigma_recover - sigma_min) / (epsilon_recover - epsilon_min);
				sigma = sigma_recover + (epsilon - epsilon_recover)*E;

				if (sigma < sigma_min)
				{
					status = YIELD_M;
					sigma = -fy + (epsilon - epsilon_y)*E1;
					E = E1;
					sigma_min_ = sigma;
					epsilon_min_ = epsilon;
				}
			}
		}
		else if (sigma_try > sigma_max)
		{
			status = YIELD_P;
			sigma = fy + (epsilon - epsilon_y)*E1;
			E = E1;
			sigma_max_ = sigma;
			epsilon_max_ = epsilon;
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
		sigma_max = sigma_max_;
		sigma_min = sigma_min_;
		epsilon_max = epsilon_max_;
		epsilon_min = epsilon_min_;
		epsilon_origin = epsilon_origin_;
	}
}