#pragma once
#include "basis.h"

enum direction { X,Y,Z,RX,RY,RZ };

class dof : public basis
{
public:
	dof(const int n, direction dir);
	dof(const int n, direction dir, const double m, const bool fixed = false);
	~dof();

	int eqnId;
	double mass;
	direction dir;
	double load;

	bool isFixed;

	double dsp, vel, acc;
};