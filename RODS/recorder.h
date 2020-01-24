#pragma once

#include <vector>
#include <armadillo>
using namespace arma;

enum response { DISP, VEL, ACC, FORCE, DEF };

class recorder
{
public:
	recorder();
	~recorder();

	void init(const int nsteps);
	virtual void record(const int cstep, const double ctime) = 0;
	void save();

	int id;
	size_t n, nsteps;
	response rtype;
	char * fileName;
	mat Res;
};