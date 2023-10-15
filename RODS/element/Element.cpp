#include "Element.h"
#include <iostream>

Element::Element(): Basis(), nv(1), force(nullptr), deformation(nullptr)
{
}

Element::Element(const int id) : Basis(id), nv(1), force(nullptr), deformation(nullptr)
{
}

Element::~Element()
{
}

void Element::printResponse()
{
	std::cout << "Response of element (ID: " << id << "): "<< std::endl;
	std::cout << "Deformation: " << std::endl;
	for (auto i = 0; i < nv; ++i)
	{
		std::cout << deformation[i] << std::endl;
	}
	std::cout << "Force: " << std::endl;
	for (auto i = 0; i < nv; ++i)
	{
		std::cout << force[i] << std::endl;
	}
}

double Element::dt = 0.0;
bool Element::isStatic = true;