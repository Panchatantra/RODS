#include "material1D.h"


material1D::material1D(const int id, const double E) :
	material(id, E), sigma(0.0), epsilon(0.0)
{
}

material1D::~material1D()
{
}
