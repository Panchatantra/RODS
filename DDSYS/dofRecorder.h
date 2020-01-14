#pragma once

#include "recorder.h"
#include "dof.h"
#include <vector>

#include <armadillo>
using namespace arma;

class dofRecorder :
	public recorder
{
public:
	dofRecorder(const int id, std::vector<dof *> dofs, char *fileName);
	~dofRecorder();

	void init(const int nsteps);
	void record(const int cstep, const double ctime);
	void save();

	std::vector<dof *> dofs;
	size_t n, nsteps;

	mat Res;
	char *fileName;
};

