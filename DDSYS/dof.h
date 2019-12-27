#pragma once

#include <string>
#include "location.h"

enum direction { X,Y,Z,RX,RY,RZ };

class dof
{
public:
	dof(const int n, location *loc, direction dir);
	dof(const int n, location *loc, direction dir, const double m, const bool fixed = false);
	~dof();

	int id;
	double mass;
	location *loc;
	direction dir;

	bool isFixed;

	double dsp, vel, acc;
	
};

