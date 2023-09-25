#include "DOFRecorder.h"

DOFRecorder::DOFRecorder(const int id, std::vector<DOF *> dofs, RODS::Response rType, const char * fileName):
	Recorder(id, rType, fileName), dofs(dofs)

{
	n = static_cast<int>(dofs.size());
}

DOFRecorder::DOFRecorder(const int id, RODS::Response rType, const char* fileName):
	Recorder(id, rType, fileName)
{
}

DOFRecorder::~DOFRecorder()
{
}

void DOFRecorder::init(const int nsteps)
{
	if (rtype == RODS::Response::ALL)
	{
		n = 3*dofs.size();
	}
	else
	{
		n = dofs.size();
	}
	Recorder::init(nsteps);
}

void DOFRecorder::record(const int cstep, const double ctime)
{
	Res(cstep, 0) = ctime;
	DOF* d;
	for (size_t i = 0; i < n; i++)
	{
		switch (rtype)
		{
		case RODS::Response::DISP:
			d = dofs[i];
			Res(cstep, i + 1) = d->dsp;
			break;
		case RODS::Response::VEL:
			d = dofs[i];
			Res(cstep, i + 1) = d->vel;
			break;
		case RODS::Response::ACC:
			d = dofs[i];
			Res(cstep, i + 1) = d->acc;
			break;
		case RODS::Response::FORCE:
			break;
		case RODS::Response::DEF:
			break;
		case RODS::Response::ALL:
		{
			auto p = i % 3;
			auto q = i / 3;
			d = dofs[q];
			if (p == 0) Res(cstep, i+1) = d->dsp;
			else if (p == 1) Res(cstep, i+1) = d->vel;
			else Res(cstep, i+1) = d->acc;
		}
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