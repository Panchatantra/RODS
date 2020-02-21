#pragma once

#include <armadillo>
#include "Basis.h"

using namespace arma;

enum response { DISP, VEL, ACC, FORCE, DEF, ALL };

class Recorder : public Basis
{
public:
	Recorder(const int id, response rtype, char * fileName);
	~Recorder();

	void init(const int nsteps);
	virtual void record(const int cstep, const double ctime) = 0;
	void save();

	void setFileName(char *fileName);

	size_t n, nsteps;
	response rtype;
	char * fileName;
	mat Res;
};