#include "Material.h"


Material::Material() :
	E(0.0)
{
}


Material::Material(const int id, const double E): Basis(id), E(E)
{
}


Material::~Material()
{
}
