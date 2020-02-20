#pragma once

#include "recorder.h"
#include "DOF.h"

class dofRecorder :
	public recorder
{
public:
	dofRecorder(const int id, std::vector<DOF *> dofs, response rtype, char * fileName);
	~dofRecorder();

	virtual void record(const int cstep, const double ctime);

	std::vector<DOF *> dofs;
};

