#include "SteelBilinear.h"
#include <iostream>

using namespace MAT_STBL;

SteelBilinear::SteelBilinear(const int id, const double E, const double fy, const double alpha, const double beta) :
    Material1D(id, E), fy(fy), alpha(alpha), sigma_ref(beta*fy),
    E0(E), epsilon_y(fy/E), E1(E*alpha),
	E_p(E), epsilon_p(0.0), sigma_p(0.0),
	sigma_max(fy), sigma_min(-fy),
    epsilon_max(fy/E), epsilon_min(-fy/E), epsilon_origin(0.0),
	epsilon_unload(0.0), sigma_unload(0.0),
	epsilon_recover(0.0), sigma_recover(0.0),
	state(ELASTIC), state_p(ELASTIC)
{
}

SteelBilinear::~SteelBilinear()
{
}

void SteelBilinear::getResponse(const bool update)
{
	double depsilon = epsilon - epsilon_p;
	double sigma_try = sigma_p + depsilon * E_p;
	sigma_max_ = sigma_max;
	sigma_min_ = sigma_min;
	epsilon_max_ = epsilon_max;
	epsilon_min_ = epsilon_min;
	double epsilon_origin_ = epsilon_origin;
	double epsilon_recover_ = epsilon_recover;
	double sigma_recover_ = sigma_recover;

	epsilon_unload_ = epsilon_unload;
	sigma_unload_ = sigma_unload;

	double sigma_r = 0.0;
	double epsilon_r = 0.0;

	state = state_p;

	switch (state)
	{
	case ELASTIC:
		if (epsilon > epsilon_y)
		{
			changeToStateYieldP();
		}
		else if (epsilon < -epsilon_y)
		{
			changeToStateYieldM();
		}
		else
		{
			changeToStateElastic();
		}
		break;
	case YIELD_P:
		if (depsilon < 0)
		{
			sigma_unload_ = sigma_p;
			epsilon_unload_ = epsilon_p;
			changeToStateUnloadP();
		}
		else
		{
			changeToStateYieldP();
		}
		break;
	case UNLOAD_P:
		sigma_r = -sigma_ref;
		epsilon_r = (sigma_r - sigma_unload_ + E0*epsilon_unload_)/E0;
		if (epsilon > epsilon_max)
		{
			changeToStateYieldP();
		}
		else if (epsilon > epsilon_unload)
		{
			changeToStateRecoverM();
		}
		else if (epsilon < epsilon_r)
		{
			changeToStateRecoverP();
		}
		else
		{
			changeToStateUnloadP();
		}
		break;
	case RECOVER_P:
		if (depsilon > 0)
		{
			sigma_unload_ = sigma_p;
			epsilon_unload_ = epsilon_p;
			changeToStateUnloadM();
		}
		else if (epsilon < epsilon_min)
		{
			changeToStateYieldM();
		}
		else
		{
			changeToStateRecoverP();
		}
		break;
	case YIELD_M:
		if (depsilon > 0)
		{
			sigma_unload_ = sigma_p;
			epsilon_unload_ = epsilon_p;
			changeToStateUnloadM();
		}
		else
		{
			changeToStateYieldM();
		}
		break;
	case UNLOAD_M:
		sigma_r = sigma_ref;
		epsilon_r = (sigma_r - sigma_unload_ + E0*epsilon_unload_)/E0;
		if (epsilon < epsilon_min)
		{
			changeToStateYieldM();
		}
		else if (epsilon < epsilon_unload)
		{
			changeToStateRecoverP();
		}
		else if (epsilon > epsilon_r)
		{
			changeToStateRecoverM();
		}
		else
		{
			changeToStateUnloadM();
		}
		break;
	case RECOVER_M:
		if (depsilon < 0)
		{
			sigma_unload_ = sigma_p;
			epsilon_unload_ = epsilon_p;
			changeToStateUnloadP();
		}
		else if (epsilon > epsilon_max)
		{
			changeToStateYieldP();
		}
		else
		{
			changeToStateRecoverM();
		}
		break;
	default:
		break;
	}

	//switch (state)
	//{
	//case ELASTIC:
	//	if (sigma_try >= fy)
	//	{
	//		state = YIELD_P;
	//		sigma = fy + (epsilon - epsilon_y)*E1;
	//		E = E1;
	//		sigma_max_ = sigma;
	//		epsilon_max_ = epsilon;
	//	}
	//	else if (sigma_try <= -fy)
	//	{
	//		state = YIELD_M;
	//		sigma = -fy + (epsilon - epsilon_y)*E1;
	//		E = E1;
	//		sigma_min_ = sigma;
	//		epsilon_min_ = epsilon;
	//	}
	//	else
	//	{
	//		sigma = sigma_try;
	//		E = E_p;
	//	}
	//	break;
	//case YIELD_P:
	//	if (depsilon < 0)
	//	{
	//		state = UNLOAD_P;
	//		E = E0;
	//		sigma = sigma_p + E * depsilon;
	//		epsilon_origin_ = epsilon_p - sigma_p / E0;

	//		if (sigma < -sigma_ref)
	//		{
	//			state = RECOVER_P;
	//			sigma_recover = -sigma_ref;
	//			epsilon_recover = epsilon_origin_ + sigma_recover / E0;
	//			E = (sigma_recover - sigma_min) / (epsilon_recover - epsilon_min);
	//			sigma = sigma_recover + (epsilon - epsilon_recover)*E;

	//			if (sigma < sigma_min)
	//			{
	//				state = YIELD_M;
	//				sigma = -fy + (epsilon - epsilon_y)*E1;
	//				E = E1;
	//				sigma_min_ = sigma;
	//				epsilon_min_ = epsilon;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		sigma = sigma_try;
	//		E = E_p;
	//		sigma_max_ = sigma;
	//		epsilon_max_ = epsilon;
	//	}
	//	break;
	//case YIELD_M:
	//	if (depsilon > 0)
	//	{
	//		state = UNLOAD_M;
	//		E = E0;
	//		sigma = sigma_p + E * depsilon;
	//		epsilon_origin_ = epsilon_p - sigma_p / E0;

	//		if (sigma > sigma_ref)
	//		{
	//			state = RECOVER_M;
	//			sigma_recover = sigma_ref;
	//			epsilon_recover = epsilon_origin_ + sigma_recover / E0;
	//			E = (sigma_recover - sigma_max) / (epsilon_recover - epsilon_max);
	//			sigma = sigma_recover + (epsilon - epsilon_recover)*E;

	//			if (sigma > sigma_max)
	//			{
	//				state = YIELD_P;
	//				sigma = fy + (epsilon - epsilon_y)*E1;
	//				E = E1;
	//				sigma_max_ = sigma;
	//				epsilon_max_ = epsilon;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		sigma = sigma_try;
	//		E = E_p;
	//		sigma_min_ = sigma;
	//		epsilon_min_ = epsilon;
	//	}
	//	break;
	//case UNLOAD_P:
	//	if (sigma_try > sigma_unload_ && sigma_try < sigma_max)
	//	{
	//		state = RECOVER_M;
	//		sigma_recover_ = sigma_ref;
	//		epsilon_recover_ = (sigma_recover_ - sigma_min_ + E0*epsilon_min_)/E0;
	//		E = (sigma_recover_ - sigma_max) / (epsilon_recover_ - epsilon_max);
	//		sigma = sigma_recover_ + (epsilon - epsilon_recover_)*E;
	//	}
	//	if (sigma_try > sigma_max)
	//	{
	//		state = YIELD_P;
	//		sigma = fy + (epsilon - epsilon_y)*E1;
	//		E = E1;
	//		sigma_max_ = sigma;
	//		epsilon_max_ = epsilon;
	//	}
	//	else if (sigma_try < -sigma_ref)
	//	{
	//		state = RECOVER_P;
	//		sigma_recover = -sigma_ref;
	//		epsilon_recover = epsilon_origin + sigma_recover / E0;
	//		E = (sigma_recover - sigma_min) / (epsilon_recover - epsilon_min);
	//		sigma = sigma_recover + (epsilon - epsilon_recover)*E;

	//		if (sigma < sigma_min)
	//		{
	//			state = YIELD_M;
	//			sigma = -fy + (epsilon - epsilon_y)*E1;
	//			E = E1;
	//			sigma_min_ = sigma;
	//			epsilon_min_ = epsilon;
	//		}
	//	}
	//	else
	//	{
	//		sigma = sigma_try;
	//		E = E_p;
	//	}
	//	break;
	//case UNLOAD_M:
	//	if (sigma_try < sigma_min)
	//	{
	//		state = YIELD_M;
	//		sigma = fy + (epsilon - epsilon_y)*E1;
	//		E = E1;
	//		sigma_min_ = sigma;
	//		epsilon_min_ = epsilon;
	//	}
	//	else if (sigma_try > sigma_ref)
	//	{
	//		state = RECOVER_M;
	//		sigma_recover = sigma_ref;
	//		epsilon_recover = epsilon_origin + sigma_recover / E0;
	//		E = (sigma_recover - sigma_max) / (epsilon_recover - epsilon_max);
	//		sigma = sigma_recover + (epsilon - epsilon_recover)*E;

	//		if (sigma > sigma_max)
	//		{
	//			state = YIELD_P;
	//			sigma = fy + (epsilon - epsilon_y)*E1;
	//			E = E1;
	//			sigma_max_ = sigma;
	//			epsilon_max_ = epsilon;
	//		}
	//	}
	//	else
	//	{
	//		sigma = sigma_try;
	//		E = E_p;
	//	}
	//	break;
	//case RECOVER_P:
	//	if (depsilon > 0)
	//	{
	//		state = UNLOAD_M;
	//		E = E0;
	//		sigma = sigma_p + E * depsilon;
	//		epsilon_origin_ = epsilon_p - sigma_p / E0;

	//		if (sigma > sigma_ref)
	//		{
	//			state = RECOVER_M;
	//			sigma_recover = sigma_ref;
	//			epsilon_recover = epsilon_origin_ + sigma_recover / E0;
	//			E = (sigma_recover - sigma_max) / (epsilon_recover - epsilon_max);
	//			sigma = sigma_recover + (epsilon - epsilon_recover)*E;

	//			if (sigma > sigma_max)
	//			{
	//				state = YIELD_P;
	//				sigma = fy + (epsilon - epsilon_y)*E1;
	//				E = E1;
	//				sigma_max_ = sigma;
	//				epsilon_max_ = epsilon;
	//			}
	//		}
	//	}
	//	else if (sigma_try < sigma_min)
	//	{
	//		state = YIELD_M;
	//		sigma = -fy + (epsilon - epsilon_y)*E1;
	//		E = E1;
	//		sigma_min_ = sigma;
	//		epsilon_min_ = epsilon;
	//	}
	//	else
	//	{
	//		sigma = sigma_try;
	//		E = E_p;
	//	}
	//	break;
	//case RECOVER_M:
	//	if (depsilon < 0)
	//	{
	//		state = UNLOAD_P;
	//		E = E0;
	//		sigma = sigma_p + E * depsilon;
	//		epsilon_origin_ = epsilon_p - sigma_p / E0;
	//		sigma_unload_ = sigma_p;

	//		if (sigma < -sigma_ref)
	//		{
	//			state = RECOVER_P;
	//			sigma_recover_ = -sigma_ref;
	//			epsilon_recover_ = epsilon_origin_ + sigma_recover_ / E0;
	//			E = (sigma_recover_ - sigma_min) / (epsilon_recover_ - epsilon_min);
	//			sigma = sigma_recover_ + (epsilon - epsilon_recover_)*E;

	//			if (sigma < sigma_min)
	//			{
	//				state = YIELD_M;
	//				sigma = -fy + (epsilon - epsilon_y)*E1;
	//				E = E1;
	//				sigma_min_ = sigma;
	//				epsilon_min_ = epsilon;
	//			}
	//		}
	//	}
	//	else if (sigma_try > sigma_max)
	//	{
	//		state = YIELD_P;
	//		sigma = fy + (epsilon - epsilon_y)*E1;
	//		E = E1;
	//		sigma_max_ = sigma;
	//		epsilon_max_ = epsilon;
	//	}
	//	else
	//	{
	//		sigma = sigma_try;
	//		E = E_p;
	//	}
	//	break;
	//default:
	//	break;
	//}

	if (update)
	{
		epsilon_p = epsilon;
		sigma_p = sigma;
		E_p = E;
		state_p = state;
		sigma_max = sigma_max_;
		sigma_min = sigma_min_;
		epsilon_max = epsilon_max_;
		epsilon_min = epsilon_min_;
		epsilon_origin = epsilon_origin_;
		epsilon_unload = epsilon_unload_;
		sigma_unload = sigma_unload_;
		epsilon_recover = epsilon_recover_;
		sigma_recover = sigma_recover_;
	}

	//std::cout << state;
}

void SteelBilinear::changeToStateElastic()
{
	state = ELASTIC;
	sigma = E0*epsilon;
	E = E0;
}

void SteelBilinear::changeToStateYieldP()
{
	state = YIELD_P;
	sigma = fy + E1*(epsilon - epsilon_y);
	E = E1;

	if (epsilon > epsilon_max)
	{
		epsilon_max_ = epsilon;
		sigma_max_ = sigma;
	}
}

void SteelBilinear::changeToStateUnloadP()
{
	state = UNLOAD_P;
	sigma = sigma_p + E0*(epsilon - epsilon_p);
	E = E0;
}

void SteelBilinear::changeToStateRecoverP()
{
	state = RECOVER_P;
	double sigma_r = -sigma_ref;
	double epsilon_r = (sigma_r - sigma_unload_ + E0*epsilon_unload_)/E0;
	E = (sigma_r - sigma_min) / (epsilon_r - epsilon_min);
	sigma = sigma_r + (epsilon - epsilon_r)*E;
}

void SteelBilinear::changeToStateYieldM()
{
	state = YIELD_M;
	sigma = -fy + E1*(epsilon + epsilon_y);
	E = E1;

	if (epsilon < epsilon_min)
	{
		epsilon_min_ = epsilon;
		sigma_min_ = sigma;
	}
}

void SteelBilinear::changeToStateUnloadM()
{
	state = UNLOAD_M;
	sigma = sigma_p + E0*(epsilon - epsilon_p);
	E = E0;
}

void SteelBilinear::changeToStateRecoverM()
{
	state = RECOVER_M;
	double sigma_r = sigma_ref;
	double epsilon_r = (sigma_r - sigma_unload_ + E0*epsilon_unload_)/E0;
	E = (sigma_r - sigma_max) / (epsilon_r - epsilon_max);
	sigma = sigma_r + (epsilon - epsilon_r)*E;
}
