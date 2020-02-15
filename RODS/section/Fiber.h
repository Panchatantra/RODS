#pragma once

#include "Basis.h"
#include "material1D.h"

class Fiber : public Basis
{
public:
	Fiber();
	Fiber(const int id, material1D *mat1D, const double A, const double y, const double z=0.0);
	~Fiber();

	void setStrain(const double *strain) { epsilon = *strain; }
	void getResponse(const bool update = false);

	Fiber *copy() {return new Fiber(id, mat1D, A, y, z);}

	material1D *mat1D;
	double A;
	double y, z;
	double N, epsilon, k;
	int nv;
	double *stress, *strain;
};