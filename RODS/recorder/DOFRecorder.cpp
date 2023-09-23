#include "DOFRecorder.h"

DOFRecorder::DOFRecorder(const int id, std::vector<DOF *> dofs, RODS::Response rType, char * fileName):
	Recorder(id, rType, fileName), dofs(dofs)

{
	n = static_cast<int>(dofs.size());
}

DOFRecorder::DOFRecorder(const int id, RODS::Response rType, char* fileName):
	Recorder(id, rType, fileName)
{
}

DOFRecorder::~DOFRecorder()
{
}

void DOFRecorder::record(const int cstep, const double ctime)
{
	Res(cstep, 0) = ctime;
	for (size_t i = 0; i < n; i++)
	{
		DOF * d = dofs[i];
		switch (rtype)
		{
		case RODS::Response::DISP:
			Res(cstep, i + 1) = d->dsp;
			break;
		case RODS::Response::VEL:
			Res(cstep, i + 1) = d->vel;
			break;
		case RODS::Response::ACC:
			Res(cstep, i + 1) = d->acc;
			break;
		case RODS::Response::FORCE:
			break;
		case RODS::Response::DEF:
			break;
		default:
			break;
		}
	}
}

void DOFRecorder::add_dof(DOF* dof)
{
	auto it = std::find(dofs.begin(), dofs.end(), dof);
	if (it == dofs.end())
	{
		dofs.push_back(dof);
		n = static_cast<int>(dofs.size());
	}
}

void DOFRecorder::get_dof_id(int *id)
{
	int i = 0;
	for (auto &it : dofs)
	{
		id[i++] = it->id;
	}
}
