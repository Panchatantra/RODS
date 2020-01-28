#pragma once

#include "recorder.h"
#include "dof.h"

class dofRecorder :
	public recorder
{
public:
	dofRecorder(const int id, std::vector<dof *> dofs, response rtype, char * fileName);
	~dofRecorder();

	virtual void record(const int cstep, const double ctime);

	std::vector<dof *> dofs;
};

