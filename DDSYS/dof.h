#pragma once

#include <string>
#include "node.h"

enum direction { X,Y,Z,RX,RY,RZ };

class dof
{
public:
	dof(const int n, node *loc, direction dir);
	dof(const int n, node *loc, direction dir, const double m, const bool fixed = false);
	~dof();

	int id;
	double mass;
	node *loc;
	direction dir;

	bool isFixed;

	double dsp, vel, acc;
	
};

