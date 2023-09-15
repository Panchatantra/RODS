#pragma once

#include "Basis.h"

/**
 * @brief      The node.
 */
class Point : public Basis
{
public:
	Point(const int id, const double x=0.0, const double y=0.0, const double z=0.0);
	~Point();

	double x, y, z;
};

