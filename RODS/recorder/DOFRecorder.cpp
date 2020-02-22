#include "DOFRecorder.h"

DOFRecorder::DOFRecorder(const int id, std::vector<DOF *> dofs, Response rtype, char * fileName):
	Recorder(id, rtype, fileName), dofs(dofs)

{
	n = dofs.size();
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
		case Response::DISP:
			Res(cstep, i + 1) = d->dsp;
			break;
		case Response::VEL:
			Res(cstep, i + 1) = d->vel;
			break;
		case Response::ACC:
			Res(cstep, i + 1) = d->acc;
			break;
		case Response::FORCE:
			break;
		case Response::DEF:
			break;
		default:
			break;
		}
	}
}