#pragma once
#include "basis.h"
#include <armadillo>

using namespace arma;

class element : public basis
{
public:
	element();
	element(const int id) : basis(id) {}
	~element();

	virtual void getResponse(const bool update = false) = 0;

	static double dt;
	int nv;
	double *force, *deformation;
};

