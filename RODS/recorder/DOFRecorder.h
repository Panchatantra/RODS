#pragma once

#include "Recorder.h"
#include "DOF.h"

/**
 * @brief      The DOF response recorder.
 */
class DOFRecorder :
	public Recorder
{
public:
	DOFRecorder(const int id, std::vector<DOF *> dofs, RODS::Response rType, char * fileName);
	~DOFRecorder();

	void record(const int cstep, const double ctime) override;

	std::vector<DOF *> dofs;
};

