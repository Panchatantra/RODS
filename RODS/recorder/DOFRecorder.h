#pragma once

#include "Recorder.h"
#include "DOF.h"

class DOFRecorder :
	public Recorder
{
public:
	DOFRecorder(const int id, std::vector<DOF *> dofs, response rtype, char * fileName);
	~DOFRecorder();

	void record(const int cstep, const double ctime) override;

	std::vector<DOF *> dofs;
};

