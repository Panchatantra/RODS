#pragma once

#include "basis.h"
#include "material.h"

class section : public basis
{
public:
	section();
	section(const int id, material *mat, const double A);
	~section();

	material *mat;
	double A;
};