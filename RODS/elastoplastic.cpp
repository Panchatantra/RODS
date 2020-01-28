#include "elastoplastic.h"



elastoplastic::elastoplastic(const int id, const double E, const double fy, const double alpha):
	material1D(id, E), fy(fy), alpha(alpha), E_t(E), epsilon_y(fy/E), E1(E*alpha)
{
}


elastoplastic::~elastoplastic()
{
}
