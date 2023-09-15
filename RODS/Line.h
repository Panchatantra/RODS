#pragma once
#include "Basis.h"
#include "Point.h"

/**
 * @brief      The line.
 */
class Line :
	public Basis
{
public:
	Line(const int id, Point *i, Point *j);
	~Line();

	Point *pointI, *pointJ;
};

