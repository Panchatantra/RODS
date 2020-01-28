#include "material.h"


material::material() :
	E(0.0)
{
}


material::material(const int id, const double E): basis(id), E(E)
{
}


material::~material()
{
}
