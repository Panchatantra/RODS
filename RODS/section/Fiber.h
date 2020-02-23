#pragma once

#include "Basis.h"
#include "Material1D.h"

/**
 * @brief      The fiber of a material.
 */
class Fiber : public Basis
{
public:
	Fiber();
	Fiber(const int id, Material1D *mat1D, const double A, const double y, const double z=0.0);
	~Fiber();

	void setStrain(const double *strain) { epsilon = *strain; }
	void getResponse(const bool update = false);

	Fiber *copy() {return new Fiber(id, mat1D, A, y, z);}

	Material1D *mat1D;
	double A;
	double y, z;
	double N, epsilon, k;
	int nv;
	double *stress, *strain;
};