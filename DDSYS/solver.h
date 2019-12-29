#pragma once

#include "dsystem.h"

class solver
{
public:
	solver();
	solver(dsystem *ds);
	~solver();

	dsystem *ds;
};


solver::solver()
{
	ds = new dsystem();
}

solver::solver(dsystem *ds)
{
	this->ds = ds;
}


solver::~solver()
{
}
