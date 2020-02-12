#pragma once
#include "basis.h"

constexpr bool FIXED = true;
enum direction { X,Y,Z,RX,RY,RZ };

class dof : public basis
{
public:
	dof(const int n, direction dir);
	dof(const int n, direction dir, const double m, const bool fixed = false);
	~dof();

	void setResponse(const double dsp, const double vel, const double acc);
	void setLoad(const double load);
	void addLoad(const double load);

    int eqnId;
	double mass;
	direction dir;
	double load;
	bool isFixed;

	double dsp, vel, acc;


	static double g;
};
