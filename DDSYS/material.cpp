#include "material.h"


material::material() :
	id(0), E(0.0)
{
}


material::material(const int id, const double E)
{
	this->id = id;
	this->E = E;
	this->E0 = E;
}


material::~material()
{
}
