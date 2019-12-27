#pragma once

#include "dsystem.h"

class solver
{
public:
	solver(dsystem *ds);
	~solver();

	dsystem *ds;
};



solver::solver(dsystem *ds)
{
	this->ds = ds;
}


solver::~solver()
{
}
