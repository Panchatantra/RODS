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
	DOFRecorder();
	DOFRecorder(const int id, std::vector<DOF *> dofs, RODS::Response rType, const char * fileName);
	DOFRecorder(const int id, RODS::Response rType, const char * fileName);
	~DOFRecorder();

	void init(const int nsteps);
	void record(const int cstep, const double ctime) override;
	void add_dof(DOF *dof);
	void remove_dof(DOF *dof);
	void get_dof_id(int *id);

	std::vector<DOF *> dofs;
	std::vector<int> DOFs;

	friend void to_json(json& jsonObj, const DOFRecorder& DOFRecorderObj) {
		jsonObj["id"] = DOFRecorderObj.id;
		jsonObj["Dofs"] = DOFRecorderObj.DOFs;
		jsonObj["rtype"] = DOFRecorderObj.rtype;
		jsonObj["fileName"] = DOFRecorderObj.fileName;
	}
	friend void from_json(const json& jsonObj, DOFRecorder& DOFRecorderObj) {
		jsonObj.at("id").get_to(DOFRecorderObj.id);
		jsonObj.at("DOFs").get_to(DOFRecorderObj.DOFs);
		jsonObj.at("rtype").get_to(DOFRecorderObj.rtype);
		jsonObj.at("fileName").get_to(DOFRecorderObj.fileName);
	};
};

