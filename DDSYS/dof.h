#pragma once

#include <string>

class dof
{
public:
	dof(const int n);
	dof(const int n, const double m, const bool fixed = false);
	dof(const int n, const bool rel);
	~dof();

	int num;
	double mass;

	bool isFixed;
	bool isRelative; // for inerter system only

	double dsp, vel, acc;
	
};

