#pragma once

#include <string>
#include "node.h"

class dof
{
public:
	dof(const int n);
	dof(const int n, const double m, const bool fixed = false);
	~dof();

	int num;
	double mass;
	node *loc;

	bool isFixed;

	double dsp, vel, acc;
	
};

