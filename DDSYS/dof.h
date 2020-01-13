#pragma once

#include <string>

enum direction { X,Y,Z,RX,RY,RZ };

class dof
{
public:
	dof(const int n, direction dir);
	dof(const int n, direction dir, const double m, const bool fixed = false);
	~dof();

	int id;
	int eqnId;
	double mass;
	direction dir;
	double dt;

	bool isFixed;

	double dsp, vel, acc;
};

