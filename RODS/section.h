#pragma once

#include "material.h"

class section
{
public:
	section();
	section(const int id, material *mat, const double A);
	~section();

	int id;
	material *mat;
	double A;
};