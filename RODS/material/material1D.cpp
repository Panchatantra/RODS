#include "Material1D.h"


Material1D::Material1D(const int id, const double E) :
	Material(id, E), sigma(0.0), epsilon(0.0)
{
}

Material1D::~Material1D()
{
}
