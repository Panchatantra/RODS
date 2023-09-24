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
	DOFRecorder(const int id, std::vector<DOF *> dofs, RODS::Response rType, const char * fileName);
	DOFRecorder(const int id, RODS::Response rType, const char * fileName);
	~DOFRecorder();

	void init(const int nsteps);
	void record(const int cstep, const double ctime) override;
	void add_dof(DOF *dof);
	void get_dof_id(int *id);

	std::vector<DOF *> dofs;
};

