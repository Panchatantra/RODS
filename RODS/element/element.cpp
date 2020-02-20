#include "element.h"
#include <iostream>

element::element()
{
	nv = 1;
}

element::element(const int id) : Basis(id)
{
	nv = 1;
}

element::~element()
{
}

void element::printResponse()
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

double element::dt = 0.0;
bool element::isStatic = false;