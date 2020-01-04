#include "material.h"



material::material() :
	E(0.0), nv(0.0), G(0.0)
{
}


material::material(const double E, const double nv, const double G)
{
	this->E = E;
	this->nv = nv;
	this->G = G;
}


material::~material()
{
}
