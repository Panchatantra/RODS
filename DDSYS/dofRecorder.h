#pragma once

#include "recorder.h"
#include "dof.h"

#include <armadillo>

using namespace arma;

class dofRecorder :
	public recorder
{
public:
	dofRecorder(dof *dofs, const int n, char *fileName, const int nsteps);
	~dofRecorder();

	void record(const int cstep, const double ctime);
	void save();

	dof *dofs;
	int n, nsteps;

	mat Res;
	char *fileName;
};

