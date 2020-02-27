#include "CyclicHardenTrilinear.h"

using namespace MAT_CHT;

CyclicHardenTrilinear::CyclicHardenTrilinear(const int id, const double E, const double sigma1, const double alpha1,
							const double sigma2, const double alpha2) :
    Material1D(id, E), sigma1(sigma1), alpha1(alpha1), sigma2(sigma2), alpha2(alpha2),
	E0(E), E1(E*alpha1), E2(E*alpha2), epsilon1(sigma1/E), epsilon2(sigma1/E+(sigma2-sigma1)/E1),
	sigma_max(sigma1), epsilon_max(sigma1/E),
	sigma_min(-sigma1), epsilon_min(-sigma1/E),
	sigma_rp(0.0), epsilon_rp(0.0),
	sigma_rn(0.0), epsilon_rn(0.0), epsilon_origin(0.0),
	state(Elastic), state_p(Elastic)
{
}

CyclicHardenTrilinear::~CyclicHardenTrilinear()
{
}

void CyclicHardenTrilinear::getResponse(const bool update)
{
	double depsilon = epsilon - epsilon_p;
	sigma_max_ = sigma_max;
	sigma_min_ = sigma_min;
	epsilon_max_ = epsilon_max;
	epsilon_min_ = epsilon_min;

	state = state_p;

	switch (state)
	{
	case Elastic:

		if (epsilon > 0)
		{
			if (epsilon > epsilon2)
			{
				changeToStateYield2P();
			}
			else if (epsilon > epsilon1)
			{
				changeToStateYield1P();
			}
			else
			{
				changeToStateElastic();
			}
		}
		else
		{
			if (epsilon < -epsilon2)
			{
				changeToStateYield2M();
			}
			else if (epsilon < -epsilon1)
			{
				changeToStateYield1M();
			}
			else
			{
				changeToStateElastic();
			}
		}
		break;
	case Yield1P:
		if (depsilon < 0)
		{
			changeToStateUnloadP();
			/// @todo large depsilon
		}
		else if (epsilon > epsilon2)
		{
			changeToStateYield2P();
		}
		else
		{
			changeToStateYield1P();
		}
		break;
	case Yield2P:
		if (depsilon < 0)
		{
			changeToStateUnloadP();
			/// @todo large depsilon
		}
		else
		{
			changeToStateYield2P();
		}
		break;
	case UnloadP:
		epsilon_origin = epsilon_p - sigma_p/E0;
		epsilon_rp = (E0*epsilon_origin + sigma_min_ - E2*epsilon_min_)/(E0-E2);
		sigma_rp = E0*(epsilon_rp - epsilon_origin);
		if (depsilon > 0)
		{
			if (epsilon > epsilon2)
			{
				changeToStateYield2P();
			}
			else
			{
				changeToStateYield1P();
			}
		}
		else if (epsilon < epsilon_rp)
		{
			changeToStateRecoverP();
			/// @todo large depsilon
		}
		else
		{
			changeToStateUnloadP();
		}
		break;
	case RecoverP:
		if (depsilon > 0)
		{
			if (epsilon < -epsilon1)
			{
				changeToStateUnloadM();
				/// @todo large depsilon
			}
			else
			{
				changeToStateUnloadP();
				/// @todo large depsilon
			}
		}
		else if (epsilon < epsilon_min_ && epsilon < -epsilon2)
		{
			changeToStateYield2M();
		}
		else if (epsilon < epsilon_min_ && epsilon < -epsilon1)
		{
			changeToStateYield1M();
		}
		else
		{
			changeToStateRecoverP();
		}
		break;
	case Yield1M:
		if (depsilon > 0)
		{
			changeToStateUnloadM();
			/// @todo large depsilon
		}
		else if (epsilon < -epsilon2)
		{
			changeToStateYield2M();
		}
		else
		{
			changeToStateYield1M();
		}
		break;
	case Yield2M:
		if (depsilon > 0)
		{
			changeToStateUnloadM();
			/// @todo large depsilon
		}
		else
		{
			changeToStateYield2M();
		}
		break;
	case UnloadM:
		epsilon_origin = epsilon_p - sigma_p/E0;
		epsilon_rn = -(E0*epsilon_origin + sigma_max_ - E2*epsilon_max_)/(E2-E0);
		sigma_rn = E0*(epsilon_rn - epsilon_origin);
		if (depsilon < 0)
		{
			if (epsilon < -epsilon2)
			{
				changeToStateYield2M();
			}
			else
			{
				changeToStateYield1M();
			}
		}
		else if (epsilon > epsilon_rn)
		{
			changeToStateRecoverM();
			/// @todo large depsilon
		}
		else
		{
			changeToStateUnloadM();
		}
		break;
	case RecoverM:
		if (depsilon < 0)
		{
			if (epsilon > epsilon1)
			{
				changeToStateUnloadP();
				/// @todo large depsilon
			}
			else
			{
				changeToStateUnloadM();
				/// @todo large depsilon
			}
		}
		else if (epsilon > epsilon_max_ && epsilon > epsilon2)
		{
			changeToStateYield2P();
		}
		else if (epsilon > epsilon_max_ && epsilon > epsilon1)
		{
			changeToStateYield1P();
		}
		else
		{
			changeToStateRecoverM();
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
		state_p = state;
		sigma_max = sigma_max_;
		sigma_min = sigma_min_;
		epsilon_max = epsilon_max_;
		epsilon_min = epsilon_min_;
	}
}

void CyclicHardenTrilinear::changeToStateElastic()
{
	state = Elastic;
	sigma = E0*epsilon;
	E = E0;
}

void CyclicHardenTrilinear::changeToStateYield1P()
{
	state = Yield1P;
	sigma = sigma1 + E1*(epsilon - epsilon1);
	E = E1;

	if (epsilon > epsilon_max)
	{
		epsilon_max_ = epsilon;
		sigma_max_ = sigma;
	}
}

void CyclicHardenTrilinear::changeToStateYield2P()
{
	state = Yield2P;
	sigma = sigma2 + E2*(epsilon - epsilon2);
	E = E2;

	if (epsilon > epsilon_max)
	{
		epsilon_max_ = epsilon;
		sigma_max_ = sigma;
	}
}

void CyclicHardenTrilinear::changeToStateUnloadP()
{
	state = UnloadP;
	sigma = sigma_p + E0*(epsilon - epsilon_p);
	E = E0;
}

void CyclicHardenTrilinear::changeToStateRecoverP()
{
	state = RecoverP;
	sigma = sigma_rp + E2*(epsilon - epsilon_rp);
	E = E2;
}

void CyclicHardenTrilinear::changeToStateYield1M()
{
	state = Yield1M;
	sigma = -sigma1 + E1*(epsilon + epsilon1);
	E = E1;

	if (epsilon < epsilon_min)
	{
		epsilon_min_ = epsilon;
		sigma_min_ = sigma;
	}
}

void CyclicHardenTrilinear::changeToStateYield2M()
{
	state = Yield2M;
	sigma = -sigma2 + E2*(epsilon + epsilon2);
	E = E2;
	
	if (epsilon < epsilon_min)
	{
		epsilon_min_ = epsilon;
		sigma_min_ = sigma;
	}
}

void CyclicHardenTrilinear::changeToStateUnloadM()
{
	state = UnloadM;
	sigma = sigma_p + E0*(epsilon - epsilon_p);
	E = E0;
}

void CyclicHardenTrilinear::changeToStateRecoverM()
{
	state = RecoverM;
	sigma = sigma_rn + E2*(epsilon - epsilon_rn);
	E = E2;
}