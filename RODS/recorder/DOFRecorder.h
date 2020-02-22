#pragma once

#include "Recorder.h"
#include "DOF.h"

class dofRecorder :
	public Recorder
{
public:
	dofRecorder(const int id, std::vector<DOF *> dofs, response rtype, char * fileName);
	~dofRecorder();

	virtual void record(const int cstep, const double ctime);

	std::vector<DOF *> dofs;
};

