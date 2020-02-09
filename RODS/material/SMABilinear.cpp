#include "SMABilinear.h"
#include <math.h>

using namespace MAT_SMABL;

SMABilinear::SMABilinear(const int id, const double E, const double fy, const double alpha, const double sigma_shift) :
	material1D(id, E), fy(fy), alpha(alpha), E0(E), epsilon_y(fy / E), E1(E*alpha), 
	sigma_shift(sigma_shift), epsilon_shift(sigma_shift/E0), sigma_recover(fy-sigma_shift),
    E_p(E), epsilon_p(0.0), sigma_p(0.0),
    sigma_max(fy), sigma_min(-fy), epsilon_max(fy/E), epsilon_min(-fy/E), epsilon_reload(0.0),
	status(ELASTIC), status_p(ELASTIC)
{
}

SMABilinear::~SMABilinear()
{
}

void SMABilinear::getResponse(const bool update)
{
	double depsilon = epsilon - epsilon_p;
	sigma = sigma_p + depsilon * E_p;
	status = status_p;
	E = E_p;

	double sigma_max_ = sigma_max;
	double sigma_min_ = sigma_min;
	double epsilon_max_ = epsilon_max;
	double epsilon_min_ = epsilon_min;
	double epsilon_reload_ = epsilon_reload;

	double epsilon_recover = 0.0;

	if ( epsilon >= 0 )
	{
		if (epsilon_p < 0) status = ELASTIC;

		switch (status)
		{
		case ELASTIC:
			if ( sigma >= fy )
			{
				status = YIELD;
				E = E1;
				sigma = fy + (epsilon - epsilon_y)*E;
				if (epsilon > epsilon_max)
				{
					sigma_max_ = sigma;
					epsilon_max_ = epsilon;
				}
			}
			break;
		case YIELD:
			if ( depsilon < 0 ) // unload from yield
			{
				status = UNLOAD;
				E = E0;
				epsilon_recover = epsilon_max - sigma_shift / E0;
				if (epsilon > epsilon_recover)  // normal unload
				{
					sigma = sigma_p + depsilon * E;
				}
				else if (epsilon > (epsilon_y - epsilon_shift))  // unload over recover point
				{
					status = RECOVER;
					E = E1;
					sigma = sigma_recover + (epsilon - sigma_recover / E0)*E;
				}
				else // unload to elastic
				{
					status = ELASTIC;
					E = E0;
					sigma = epsilon * E;
				}
			}
			else
			{
				sigma_max_ = sigma;
				epsilon_max_ = epsilon;
			}
			break;
		case UNLOAD:
			epsilon_recover = epsilon_max - sigma_shift / E0;
			if (epsilon > epsilon_max) // Reload to yield
			{
				status = YIELD;
				E = E1;
				sigma = fy + (epsilon - epsilon_y)*E;
				sigma_max_ = sigma;
				epsilon_max_ = epsilon;
			}
			else if (epsilon < epsilon_recover && epsilon >= (epsilon_y - epsilon_shift))  // unload over recover point
			{
				status = RECOVER;
				E = E1;
				sigma = sigma_recover + (epsilon - sigma_recover / E0)*E;
			}
			else if (epsilon < (epsilon_y - epsilon_shift)) // unload to elastic
			{
				status = ELASTIC;
				E = E0;
				sigma = epsilon * E;
			}
			break;
		case RECOVER:
			if ( depsilon>0 )
			{
				status = RELOAD;
				E = E0;
				epsilon_reload_ = epsilon_p;
				sigma = sigma_p + depsilon * E;
				if (sigma > sigma_p + sigma_shift)
				{
					status = YIELD;
					E = E1;
					sigma = fy + (epsilon - epsilon_y)*E;
					if (epsilon > epsilon_max)
					{
						sigma_max_ = sigma;
						epsilon_max_ = epsilon;
					}
				}
			}
			else if ( epsilon < (epsilon_y - epsilon_shift) ) // unload to elastic
			{
				status = ELASTIC;
				E = E0;
				sigma = epsilon * E;
			}
			break;
		case RELOAD:
			if (epsilon > epsilon_reload + epsilon_shift)
			{
				status = YIELD;
				E = E1;
				sigma = fy + (epsilon - epsilon_y)*E;
				if (epsilon > epsilon_max)
				{
					sigma_max_ = sigma;
					epsilon_max_ = epsilon;
				}
			}
			else if (epsilon < epsilon_reload)
			{
				if (epsilon > (epsilon_y - epsilon_shift))
				{
					status = RECOVER;
					E = E1;
					sigma = sigma_recover + (epsilon - sigma_recover / E0)*E;
				}
				else
				{
					status = ELASTIC;
					E = E0;
					sigma = epsilon * E;
				}
			}
			break;
		default:
			break;
		}
	}
	else
	{
		if (epsilon_p > 0) status = ELASTIC;

		switch (status)
		{
		case ELASTIC:
			if ( sigma <= -fy )
			{
				status = YIELD;
				E = E1;
				sigma = -fy + (epsilon + epsilon_y)*E;
				if (epsilon < epsilon_min)
				{
					sigma_min_ = sigma;
					epsilon_min_ = epsilon;
				}
			}
			break;
		case YIELD:
			if ( depsilon > 0 ) // unload from yield
			{
				status = UNLOAD;
				E = E0;
				epsilon_recover = epsilon_min + sigma_shift / E0;
				if (epsilon < epsilon_recover)  // normal unload
				{
					sigma = sigma_p + depsilon * E;
				}
				else if (epsilon < -(epsilon_y - epsilon_shift))  // unload over recover point
				{
					status = RECOVER;
					E = E1;
					sigma = -sigma_recover + (epsilon + sigma_recover / E0)*E;
				}
				else // unload to elastic
				{
					status = ELASTIC;
					E = E0;
					sigma = epsilon * E;
				}
			}
			else
			{
				sigma_min_ = sigma;
				epsilon_min_ = epsilon;
			}
			break;
		case UNLOAD:
			epsilon_recover = epsilon_min + sigma_shift / E0;
			if (epsilon < epsilon_min) // Reload to yield
			{
				status = YIELD;
				E = E1;
				sigma = -fy + (epsilon + epsilon_y)*E;
				sigma_min_ = sigma;
				epsilon_min_ = epsilon;
			}
			else if (epsilon > epsilon_recover && epsilon <= -(epsilon_y - epsilon_shift))  // unload over recover point
			{
				status = RECOVER;
				E = E1;
				sigma = -sigma_recover + (epsilon + sigma_recover / E0)*E;
			}
			else if (epsilon > -(epsilon_y - epsilon_shift)) // unload to elastic
			{
				status = ELASTIC;
				E = E0;
				sigma = epsilon * E;
			}
			break;
		case RECOVER:
			if ( depsilon < 0 )
			{
				status = RELOAD;
				E = E0;
				epsilon_reload_ = epsilon_p;
				sigma = sigma_p + depsilon * E;
				if (sigma < sigma_p - sigma_shift)
				{
					status = YIELD;
					E = E1;
					sigma = -fy + (epsilon + epsilon_y)*E;
					if (epsilon < epsilon_min)
					{
						sigma_min_ = sigma;
						epsilon_min_ = epsilon;
					}
				}
			}
			else if (epsilon > -(epsilon_y - epsilon_shift)) // unload to elastic
			{
				status = ELASTIC;
				E = E0;
				sigma = epsilon * E;
			}
			break;
		case RELOAD:
			if (epsilon > epsilon_reload - epsilon_shift)
			{
				status = YIELD;
				E = E1;
				sigma = -fy + (epsilon + epsilon_y)*E;
				if (epsilon < epsilon_min)
				{
					sigma_min_ = sigma;
					epsilon_min_ = epsilon;
				}
			}
			else if (epsilon > epsilon_reload)
			{
				if (epsilon < -(epsilon_y - epsilon_shift))
				{
					status = RECOVER;
					E = E1;
					sigma = -sigma_recover + (epsilon + sigma_recover / E0)*E;
				}
				else
				{
					status = ELASTIC;
					E = E0;
					sigma = epsilon * E;
				}
			}
			break;
		default:
			break;
		}
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
		epsilon_reload = epsilon_reload_;
	}
}
