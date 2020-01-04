#pragma once

#include "material.h"

class section
{
public:
	section();
	section(material *mat, const double A);
	~section();

	material *mat;
	double A;
};