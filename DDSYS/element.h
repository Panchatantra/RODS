#pragma once
#include <armadillo>

using namespace arma;

class element
{
public:
	element();
	~element();

	virtual void getResponse(const bool update = false) = 0;

	static double dt;
	int id;
	int nv;
	double *force, *deformation;
};

