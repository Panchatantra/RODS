#pragma once

#include <vector>
#include <armadillo>
#include "basis.h"

using namespace arma;

enum response { DISP, VEL, ACC, FORCE, DEF };

class recorder : public basis
{
public:
	recorder();
	~recorder();

	void init(const int nsteps);
	virtual void record(const int cstep, const double ctime) = 0;
	void save();

	void setFileName(char *fileName);

	size_t n, nsteps;
	response rtype;
	char * fileName;
	mat Res;
};